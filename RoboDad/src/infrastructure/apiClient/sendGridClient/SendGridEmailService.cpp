#include "infrastructure/apiClient/sendGridClient/SendGridEmailService.h"

SendGridEmailService::SendGridEmailService(const std::string& apiKey, const std::string& fromAddress, const std::string& appBaseUrl)
    : ApiClient(apiKey, "https://api.sendgrid.com"), fromAddress_(fromAddress), appBaseUrl_(appBaseUrl) {}

void SendGridEmailService::sendVerificationEmail(const std::string& toEmail, const std::string& token) {
    if (!isConfigured()) return;

    const std::string verifyUrl = appBaseUrl_ + "/auth/verify?token=" + token;
    const std::string body =
        "{\"personalizations\":[{\"to\":[{\"email\":\"" + toEmail + "\"}]}],"
        "\"from\":{\"email\":\"" + fromAddress_ + "\"},"
        "\"subject\":\"Verify your RoboDad email address\","
        "\"content\":[{\"type\":\"text/plain\",\"value\":"
        "\"Welcome to RoboDad! Please verify your email address by clicking the link below:\\n\\n"
        + verifyUrl + "\\n\\nThis link expires in 24 hours.\"}]}";

    httpPost(
        baseUrl_ + "/v3/mail/send",
        body,
        {"Content-Type: application/json", "Authorization: Bearer " + apiKey_}
    );
}
