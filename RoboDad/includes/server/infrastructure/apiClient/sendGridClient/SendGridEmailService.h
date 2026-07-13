#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "application/ports/IEmailService.h"
#include <string>

class SendGridEmailService : public ApiClient, public IEmailService {
    std::string fromAddress_;
    std::string appBaseUrl_;

public:
    SendGridEmailService(const std::string& apiKey, const std::string& fromAddress, const std::string& appBaseUrl);

    void sendVerificationEmail(const std::string& toEmail, const std::string& token) override;
};
