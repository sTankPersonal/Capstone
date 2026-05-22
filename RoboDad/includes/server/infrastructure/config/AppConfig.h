#pragma once
#include <string>

class AppConfig {
public:
    static AppConfig fromEnv();

    const std::string& openAiApiKey()        const { return openAiApiKey_; }
    const std::string& openAiModel()         const { return openAiModel_; }
    const std::string& jwtSecret()           const { return jwtSecret_; }
    const std::string& googleClientId()      const { return googleClientId_; }
    const std::string& googleClientSecret()  const { return googleClientSecret_; }

private:
    std::string openAiApiKey_;
    std::string openAiModel_;
    std::string jwtSecret_;
    std::string googleClientId_;
    std::string googleClientSecret_;

    AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret,
        std::string googleClientId, std::string googleClientSecret);
};