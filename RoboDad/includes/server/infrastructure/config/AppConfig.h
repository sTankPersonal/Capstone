#pragma once
#include <string>

class AppConfig {
public:
    static AppConfig fromEnv();

    const std::string& openAiApiKey() const { return openAiApiKey_; }
    const std::string& openAiModel()  const { return openAiModel_; }
    const std::string& jwtSecret()    const { return jwtSecret_; }

private:
    std::string openAiApiKey_;
    std::string openAiModel_;
    std::string jwtSecret_;

    AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret);
};
