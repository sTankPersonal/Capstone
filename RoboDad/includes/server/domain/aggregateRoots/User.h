#pragma once

#include "IAggregateRoot.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include <chrono>
#include <optional>
#include <string>

class User : public IAggregateRoot<UserId, std::chrono::year_month_day, std::chrono::year_month_day>
{
    UserId userId_;
    UserLogin userLogin_;
    UserInformation userInformation_;
    bool isVerified_;
    std::optional<std::string> verificationToken_;
    std::optional<std::chrono::system_clock::time_point> verificationTokenExpiresAt_;
    std::chrono::year_month_day createdAt_;
    std::chrono::year_month_day updatedAt_;

public:
    explicit User(const UserId& userId, const UserLogin& userLogin, const UserInformation& userInformation,
                  const std::chrono::year_month_day& createdAt, const std::chrono::year_month_day& updatedAt,
                  bool isVerified = false,
                  const std::optional<std::string>& verificationToken = std::nullopt,
                  const std::optional<std::chrono::system_clock::time_point>& verificationTokenExpiresAt = std::nullopt);

    const UserId& getId() const noexcept override;
    const UserLogin& getUserLogin() const noexcept;
    const UserInformation& getUserInformation() const noexcept;
    bool isVerified() const noexcept;
    const std::optional<std::string>& getVerificationToken() const noexcept;
    const std::optional<std::chrono::system_clock::time_point>& getVerificationTokenExpiresAt() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
    const std::chrono::year_month_day& getUpdatedAt() const noexcept override;

    void markVerified();
    void setVerificationToken(const std::string& token, const std::chrono::system_clock::time_point& expiresAt);

protected:
    void setUserLogin(const UserLogin& userLogin);
    void setUserInformation(const UserInformation& userInformation);
    void setUpdatedAt(const std::chrono::year_month_day& updatedAt) override;
};
