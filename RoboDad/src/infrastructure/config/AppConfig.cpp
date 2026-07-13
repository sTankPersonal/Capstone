#include "infrastructure/config/AppConfig.h"
#include <cstdlib>

AppConfig::AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret,
    std::string googleClientId, std::string googleClientSecret,
    std::string plaidClientId, std::string plaidSecret, std::string plaidEnv,
    std::string sendGridApiKey, std::string emailFromAddress, std::string appBaseUrl)
    : openAiApiKey_(std::move(openAiApiKey))
    , openAiModel_(std::move(openAiModel))
    , jwtSecret_(std::move(jwtSecret))
    , googleClientId_(std::move(googleClientId))
    , googleClientSecret_(std::move(googleClientSecret))
    , plaidClientId_(std::move(plaidClientId))
    , plaidSecret_(std::move(plaidSecret))
    , plaidEnv_(std::move(plaidEnv))
    , sendGridApiKey_(std::move(sendGridApiKey))
    , emailFromAddress_(std::move(emailFromAddress))
    , appBaseUrl_(std::move(appBaseUrl)) {
}

AppConfig AppConfig::fromEnv() {
    const char* key = std::getenv("OPENAI_API_KEY");
    const char* model = std::getenv("OPENAI_MODEL");
    const char* secret = std::getenv("JWT_SECRET");
    const char* gClientId = std::getenv("GOOGLE_CLIENT_ID");
    const char* gClientSecret = std::getenv("GOOGLE_CLIENT_SECRET");
    const char* plaidId = std::getenv("PLAID_CLIENT_ID");
    const char* plaidSecret = std::getenv("PLAID_SECRET");
    const char* plaidEnv = std::getenv("PLAID_ENV");
    const char* sendGridApiKey = std::getenv("SENDGRID_API_KEY");
    const char* emailFromAddress = std::getenv("EMAIL_FROM_ADDRESS");
    const char* appBaseUrl = std::getenv("APP_BASE_URL");
    return AppConfig(
        key ? key : "",
        model ? model : "gpt-4o-mini",
        secret ? secret : "robodad-default-secret",
        gClientId ? gClientId : "",
        gClientSecret ? gClientSecret : "",
        plaidId ? plaidId : "",
        plaidSecret ? plaidSecret : "",
        plaidEnv ? plaidEnv : "sandbox",
        sendGridApiKey ? sendGridApiKey : "",
        emailFromAddress ? emailFromAddress : "no-reply@robodad.app",
        appBaseUrl ? appBaseUrl : "http://localhost:18080"
    );
}
