#include "presentation/controllers/GoogleAuthController.h"
#include "presentation/routes/Routes.h"
#include "application/auth/RegisterUser.h"
#include "UserInformation.h"
#include "EmploymentStatusId.h"
#include <crow.h>
#include <curl/curl.h>
#include <sstream>
#include <stdexcept>

// CURL write callback
static size_t writeCallback(char* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string GoogleAuthController::urlEncode(const std::string& value) {
    CURL* curl = curl_easy_init();
    if (!curl) return value;
    char* encoded = curl_easy_escape(curl, value.c_str(), static_cast<int>(value.size()));
    std::string result = encoded ? encoded : value;
    curl_free(encoded);
    curl_easy_cleanup(curl);
    return result;
}

std::string GoogleAuthController::exchangeCodeForToken(const std::string& code) {
    std::string postData =
        "code=" + urlEncode(code) +
        "&client_id=" + urlEncode(clientId_) +
        "&client_secret=" + urlEncode(clientSecret_) +
        "&redirect_uri=" + urlEncode("http://localhost:18080/auth/google/callback") +
        "&grant_type=authorization_code";

    std::string response;
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("CURL init failed");

    curl_easy_setopt(curl, CURLOPT_URL, "https://oauth2.googleapis.com/token");
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    auto json = crow::json::load(response);
    if (!json || !json.has("access_token"))
        throw std::runtime_error("Failed to get access token");

    return json["access_token"].s();
}

std::pair<std::string, std::string> GoogleAuthController::fetchGoogleUserInfo(const std::string& accessToken) {
    std::string response;
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("CURL init failed");

    std::string url = "https://www.googleapis.com/oauth2/v2/userinfo?access_token=" + urlEncode(accessToken);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    auto json = crow::json::load(response);
    if (!json || !json.has("email"))
        throw std::runtime_error("Failed to get user info");

    std::string name = json.has("name") ? std::string(json["name"].s()) : "Google User";
    std::string email = std::string(json["email"].s());
    return { name, email };
}

void GoogleAuthController::registerRoutes(crow::SimpleApp& app) {

    // Step 1: redirect to Google consent screen
    CROW_ROUTE(app, "/auth/google")
        ([this](const crow::request&, crow::response& res) {
        std::string url =
            "https://accounts.google.com/o/oauth2/v2/auth"
            "?client_id=" + urlEncode(clientId_) +
            "&redirect_uri=" + urlEncode("http://localhost:18080/auth/google/callback") +
            "&response_type=code"
            "&scope=" + urlEncode("openid email profile") +
            "&prompt=select_account";
        res.code = 302;
        res.set_header("Location", url);
        res.end();
            });

    // Step 2: handle callback from Google
    CROW_ROUTE(app, "/auth/google/callback")
        ([this](const crow::request& req, crow::response& res) {
        try {
            std::string code = req.url_params.get("code") ? req.url_params.get("code") : "";
            if (code.empty()) {
                res.code = 302;
                res.set_header("Location", "/login?error=google_failed");
                res.end();
                return;
            }

            std::string accessToken = exchangeCodeForToken(code);
            auto [name, email] = fetchGoogleUserInfo(accessToken);

            // Check if user already exists
            auto existing = repo_.lookupCredentials(email);
            if (existing) {
                // Existing user � just log them in
                res.code = 302;
                res.set_header("Set-Cookie", "token=" + jwt_.generate(existing->first.getId()) +
                    "; HttpOnly; Path=/; Max-Age=86400");
                res.set_header("Location", "/chat");
                res.end();
                return;
            }

            // New user � redirect to complete profile
            res.code = 302;
            res.set_header("Location",
                "/complete-profile?name=" + urlEncode(name) +
                "&email=" + urlEncode(email));
            res.end();

        }
        catch (const std::exception& e) {
            std::cerr << "Google auth error: " << e.what() << std::endl;
            res.code = 302;
            res.set_header("Location", "/login?error=google_failed");
            res.end();
        }
            });

    // Step 3: serve complete profile page
    CROW_ROUTE(app, "/complete-profile")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/complete-profile.html");
        res.end();
            });

    // Step 4: handle profile completion and create account
    CROW_ROUTE(app, "/api/auth/google/complete").methods("POST"_method)
        ([this](const crow::request& req) {
        try {
            const std::string bodyStr = "?" + req.body;
            crow::query_string form(bodyStr);

            std::string name = form.get("name") ? form.get("name") : "";
            std::string email = form.get("email") ? form.get("email") : "";
            int age = form.get("age") ? std::stoi(form.get("age")) : 0;
            int statusInt = form.get("employment_status") ? std::stoi(form.get("employment_status")) : 0;

            if (name.empty() || email.empty()) {
                crow::response res(302);
                res.set_header("Location", "/complete-profile?error=missing_fields");
                return res;
            }

            auto spacePos = name.find(' ');
            std::optional<std::string> firstName = spacePos != std::string::npos ? name.substr(0, spacePos) : name;
            std::optional<std::string> lastName  = spacePos != std::string::npos ? std::optional<std::string>{name.substr(spacePos + 1)} : std::nullopt;
            std::optional<EmploymentStatusId> empStatus = statusInt > 0
                ? std::optional<EmploymentStatusId>{EmploymentStatusId(std::to_string(statusInt))}
                : std::nullopt;

            UserInformation info(firstName, lastName, std::nullopt, std::nullopt, std::nullopt, std::nullopt, empStatus);

            RegisterUser registerUser(repo_, hasher_);
            auto user = registerUser.execute(email, "", info);

            crow::response res(302);
            res.set_header("Set-Cookie", "token=" + jwt_.generate(user.getId().getId()) +
                "; HttpOnly; Path=/; Max-Age=86400");
            res.set_header("Location", "/chat");
            return res;

        }
        catch (const std::exception& e) {
            std::cerr << "Google complete error: " << e.what() << std::endl;
            crow::response res(302);
            res.set_header("Location", "/complete-profile?error=failed");
            return res;
        }
            });
}