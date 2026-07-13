#include "User.h"

User::User(const UserId& userId, const UserLogin& userLogin, const UserInformation& userInformation,
           const std::chrono::year_month_day& createdAt, const std::chrono::year_month_day& updatedAt,
           bool isVerified,
           const std::optional<std::string>& verificationToken,
           const std::optional<std::chrono::system_clock::time_point>& verificationTokenExpiresAt)
    : userId_(userId), userLogin_(userLogin), userInformation_(userInformation)
    , isVerified_(isVerified), verificationToken_(verificationToken), verificationTokenExpiresAt_(verificationTokenExpiresAt)
    , createdAt_(createdAt), updatedAt_(updatedAt) {}

const UserId& User::getId() const noexcept { return userId_; }
const UserLogin& User::getUserLogin() const noexcept { return userLogin_; }
const UserInformation& User::getUserInformation() const noexcept { return userInformation_; }
bool User::isVerified() const noexcept { return isVerified_; }
const std::optional<std::string>& User::getVerificationToken() const noexcept { return verificationToken_; }
const std::optional<std::chrono::system_clock::time_point>& User::getVerificationTokenExpiresAt() const noexcept { return verificationTokenExpiresAt_; }
const std::chrono::year_month_day& User::getCreatedAt() const noexcept { return createdAt_; }
const std::chrono::year_month_day& User::getUpdatedAt() const noexcept { return updatedAt_; }

void User::markVerified() {
    isVerified_ = true;
    verificationToken_ = std::nullopt;
    verificationTokenExpiresAt_ = std::nullopt;
}

void User::setVerificationToken(const std::string& token, const std::chrono::system_clock::time_point& expiresAt) {
    verificationToken_ = token;
    verificationTokenExpiresAt_ = expiresAt;
}

void User::setUserLogin(const UserLogin& userLogin) { userLogin_ = userLogin; }
void User::setUserInformation(const UserInformation& userInformation) { userInformation_ = userInformation; }
void User::setUpdatedAt(const std::chrono::year_month_day& updatedAt) { updatedAt_ = updatedAt; }
