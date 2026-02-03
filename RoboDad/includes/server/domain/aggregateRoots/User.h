#pragma once

#include "IAggregateRoot.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include <chrono>

class User : public IAggregateRoot<UserId, std::chrono::year_month_day, std::chrono::year_month_day>
{
    UserId userId_;
    UserLogin userLogin_;
    UserInformation userInformation_;
    std::chrono::year_month_day createdAt_;
    std::chrono::year_month_day updatedAt_;

public:
    explicit User(const UserId& userId, const UserLogin& userLogin, const UserInformation& userInformation, const std::chrono::year_month_day& createdAt, const std::chrono::year_month_day& updatedAt);

    const UserId& getId() const noexcept override;
    const UserLogin& getUserLogin() const noexcept;
    const UserInformation& getUserInformation() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
    const std::chrono::year_month_day& getUpdatedAt() const noexcept override;

protected:
    void setUserLogin(const UserLogin& userLogin);
    void setUserInformation(const UserInformation& userInformation);
    void setUpdatedAt(const std::chrono::year_month_day& updatedAt) override;
};
