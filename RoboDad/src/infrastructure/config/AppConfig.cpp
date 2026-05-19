#include "infrastructure/config/AppConfig.h"
#include <cstdlib>

AppConfig::AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret)
    : openAiApiKey_(std::move(openAiApiKey))
    , openAiModel_(std::move(openAiModel))
    , jwtSecret_(std::move(jwtSecret)) {}

AppConfig AppConfig::fromEnv() {
    const char* key    = std::getenv("OPENAI_API_KEY");
    const char* model  = std::getenv("OPENAI_MODEL");
    const char* secret = std::getenv("JWT_SECRET");
    return AppConfig(
        key    ? key    : "",
        model  ? model  : "gpt-4o-mini",
        secret ? secret : "robodad-default-secret"
    );
}
