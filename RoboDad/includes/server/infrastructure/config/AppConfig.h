#pragma once
#include <string>

class AppConfig {
private:
    std::string openAiApiKey_;
    std::string openAiModel_;
    std::string jwtSecret_;
    std::string googleClientId_;
    std::string googleClientSecret_;
    std::string plaidClientId_;
    std::string plaidSecret_;
    std::string plaidEnv_;
public:
    static AppConfig fromEnv();
    const std::string& plaidClientId()  const { return plaidClientId_; }
    const std::string& plaidSecret()    const { return plaidSecret_; }
    const std::string& plaidEnv()       const { return plaidEnv_; }
    const std::string& openAiApiKey()   const { return openAiApiKey_; }
    const std::string& openAiModel()    const { return openAiModel_; }
    const std::string& jwtSecret()      const { return jwtSecret_; }
    const std::string& googleClientId()      const { return googleClientId_; }
    const std::string& googleClientSecret()  const { return googleClientSecret_; }



    AppConfig(std::string openAiApiKey, std::string openAiModel, std::string jwtSecret,
        std::string googleClientId, std::string googleClientSecret,
        std::string plaidClientId, std::string plaidSecret, std::string plaidEnv);
};
