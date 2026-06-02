#include "presentation/controllers/AuthenticationController.h"
#include "domain/valueObjects/UserInformation.h"
#include <crow.h>
#include <stdexcept>
#include <string>

std::optional<std::string> AuthenticationController::extractUserId(const crow::request& req) const {
    std::string cookie = req.get_header_value("Cookie");
    const std::string prefix = "token=";
    auto pos = cookie.find(prefix);
    if (pos == std::string::npos) return std::nullopt;
    pos += prefix.size();
    auto end = cookie.find(';', pos);
    std::string token = (end == std::string::npos)
        ? cookie.substr(pos)
        : cookie.substr(pos, end - pos);
    return jwt_.verify(token);
}

void AuthenticationController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/login")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
    });

    CROW_ROUTE(app, "/signup")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/signup.html");
        res.end();
    });

    CROW_ROUTE(app, "/logout")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
    });

    CROW_ROUTE(app, "/api/auth/login").methods("POST"_method)
    ([this](const crow::request& req) {
        try {
            const std::string bodyStr = "?" + req.body;
            crow::query_string form(bodyStr);
            std::string email    = form.get("email")    ? form.get("email")    : "";
            std::string password = form.get("password") ? form.get("password") : "";

            auto user = loginUser_.execute(email, password);
            if (!user) {
                crow::response res(302);
                res.set_header("Location", "/login?error=invalid");
                return res;
            }

            crow::response res(302);
            res.set_header("Set-Cookie",
                "token=" + jwt_.generate(user->getId().getId()) + "; HttpOnly; Path=/");
            res.set_header("Location", "/chat");
            return res;
        } catch (const std::exception&) {
            crow::response res(302);
            res.set_header("Location", "/login?error=failed");
            return res;
        }
    });

    CROW_ROUTE(app, "/api/auth/logout").methods("POST"_method)
    ([]() {
        return crow::response(200, R"({"message":"Logged out"})");
    });

    CROW_ROUTE(app, "/api/auth/register").methods("POST"_method)
    ([this](const crow::request& req) {
        try {
            const std::string bodyStr = "?" + req.body;
            crow::query_string form(bodyStr);

            std::string email    = form.get("email")     ? form.get("email")    : "";
            std::string password = form.get("password")  ? form.get("password") : "";

            std::optional<std::string> firstName = form.get("firstName")
                ? std::optional<std::string>(form.get("firstName")) : std::nullopt;
            std::optional<std::string> lastName = form.get("lastName")
                ? std::optional<std::string>(form.get("lastName")) : std::nullopt;

            UserInformation info(firstName, lastName,
                                 std::nullopt, std::nullopt, std::nullopt,
                                 std::nullopt, std::nullopt);

            auto user = registerUser_.execute(email, password, info);

            crow::response res(302);
            res.set_header("Set-Cookie",
                "token=" + jwt_.generate(user.getId().getId()) + "; HttpOnly; Path=/");
            res.set_header("Location", "/chat");
            return res;
        } catch (const std::exception& e) {
            std::string msg = e.what();
            std::string location = msg.find("duplicate key") != std::string::npos
                ? "/signup?error=email_taken"
                : "/signup?error=failed";
            crow::response res(302);
            res.set_header("Location", location);
            return res;
        }
    });
}
