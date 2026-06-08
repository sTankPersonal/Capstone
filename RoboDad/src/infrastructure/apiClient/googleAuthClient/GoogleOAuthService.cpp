#include "infrastructure/apiClient/googleAuthClient/GoogleOAuthService.h"
#include <crow.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>

static std::string urlEncode(const std::string& input) {
    std::ostringstream out;
    for (unsigned char c : input) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            out << c;
        } else {
            out << '%' << std::uppercase << std::hex
                << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
    }
    return out.str();
}

GoogleOAuthService::GoogleOAuthService(const std::string& clientId,
                                       const std::string& clientSecret,
                                       const std::string& redirectUri)
    : ApiClient(clientId, "https://oauth2.googleapis.com")
    , clientSecret_(clientSecret)
    , redirectUri_(redirectUri) {}

std::string GoogleOAuthService::buildAuthorizationUrl() const {
    return "https://accounts.google.com/o/oauth2/v2/auth"
           "?client_id="      + urlEncode(apiKey_) +
           "&redirect_uri="   + urlEncode(redirectUri_) +
           "&response_type=code"
           "&scope="          + urlEncode("openid email profile") +
           "&access_type=offline";
}

std::string GoogleOAuthService::exchangeCodeForToken(const std::string& code) {
    const std::string body =
        "code="           + urlEncode(code) +
        "&client_id="     + urlEncode(apiKey_) +
        "&client_secret=" + urlEncode(clientSecret_) +
        "&redirect_uri="  + urlEncode(redirectUri_) +
        "&grant_type=authorization_code";

    const std::string raw = httpPost(
        baseUrl_ + "/token",
        body,
        {"Content-Type: application/x-www-form-urlencoded"}
    );

    auto json = crow::json::load(raw);
    if (!json || !json.has("access_token")) {
        throw std::runtime_error("GoogleOAuthService: missing access_token in token response");
    }
    return std::string(json["access_token"].s());
}

GoogleUserInfo GoogleOAuthService::fetchUserInfo(const std::string& accessToken) {
    const std::string raw = httpGet(
        "https://www.googleapis.com/oauth2/v2/userinfo",
        {"Authorization: Bearer " + accessToken}
    );

    auto json = crow::json::load(raw);
    if (!json || !json.has("email")) {
        throw std::runtime_error("GoogleOAuthService: missing email in userinfo response");
    }

    GoogleUserInfo info;
    info.googleId  = json.has("id")          ? std::string(json["id"].s())          : "";
    info.email     = std::string(json["email"].s());
    info.firstName = json.has("given_name")  ? std::string(json["given_name"].s())  : "";
    info.lastName  = json.has("family_name") ? std::string(json["family_name"].s()) : "";
    return info;
}
