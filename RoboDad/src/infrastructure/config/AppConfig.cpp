#include "infrastructure/config/AppConfig.h"
#include <cstdlib>

AppConfig::AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret,
    std::string googleClientId, std::string googleClientSecret,
    std::string plaidClientId, std::string plaidSecret)
    : openAiApiKey_(std::move(openAiApiKey))
    , openAiModel_(std::move(openAiModel))
    , jwtSecret_(std::move(jwtSecret))
    , googleClientId_(std::move(googleClientId))
    , googleClientSecret_(std::move(googleClientSecret))
    , plaidClientId_(std::move(plaidClientId))
    , plaidSecret_(std::move(plaidSecret)) {
}

AppConfig AppConfig::fromEnv() {
    const char* key = std::getenv("OPENAI_API_KEY");
    const char* model = std::getenv("OPENAI_MODEL");
    const char* secret = std::getenv("JWT_SECRET");
    const char* gClientId = std::getenv("GOOGLE_CLIENT_ID");
    const char* gClientSecret = std::getenv("GOOGLE_CLIENT_SECRET");
    const char* plaidId = std::getenv("PLAID_CLIENT_ID");
    const char* plaidSecret = std::getenv("PLAID_SECRET");
    return AppConfig(
        key ? key : "",
        model ? model : "gpt-4o-mini",
        secret ? secret : "robodad-default-secret",
        gClientId ? gClientId : "",
        gClientSecret ? gClientSecret : "",
        plaidId ? plaidId : "",
        plaidSecret ? plaidSecret : ""
    );
}
