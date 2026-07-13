#pragma once
#include <string>

class IEmailService {
public:
    virtual ~IEmailService() = default;

    // Sends a verification email containing a link built from the given token
    // to the given address.
    virtual void sendVerificationEmail(const std::string& toEmail, const std::string& token) = 0;
};
