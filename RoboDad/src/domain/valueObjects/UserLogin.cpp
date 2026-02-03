#include "UserLogin.h"

UserLogin::UserLogin(const std::string& email, const std::optional<std::string>& passwordHash)
    : email_(email), passwordHash_(passwordHash) {}

const std::string& UserLogin::getEmail() const noexcept { return email_; }
const std::optional<std::string>& UserLogin::getPasswordHash() const noexcept { return passwordHash_; }

void UserLogin::setEmail(const std::string& email) { email_ = email; }
void UserLogin::setPasswordHash(const std::optional<std::string>& passwordHash) { passwordHash_ = passwordHash; }
