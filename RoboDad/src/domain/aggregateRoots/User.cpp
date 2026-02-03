#include "User.h"

User::User(const UserId& userId, const UserLogin& userLogin, const UserInformation& userInformation, const std::chrono::year_month_day& createdAt, const std::chrono::year_month_day& updatedAt)
    : userId_(userId), userLogin_(userLogin), userInformation_(userInformation), createdAt_(createdAt), updatedAt_(updatedAt) {}

const UserId& User::getId() const noexcept { return userId_; }
const UserLogin& User::getUserLogin() const noexcept { return userLogin_; }
const UserInformation& User::getUserInformation() const noexcept { return userInformation_; }
const std::chrono::year_month_day& User::getCreatedAt() const noexcept { return createdAt_; }
const std::chrono::year_month_day& User::getUpdatedAt() const noexcept { return updatedAt_; }

void User::setUserLogin(const UserLogin& userLogin) { userLogin_ = userLogin; }
void User::setUserInformation(const UserInformation& userInformation) { userInformation_ = userInformation; }
void User::setUpdatedAt(const std::chrono::year_month_day& updatedAt) { updatedAt_ = updatedAt; }
