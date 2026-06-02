#pragma once

#include "IValueObject.h"
#include <string>
#include <optional>

class UserLogin : public IValueObject {
    std::string email_;
    std::optional<std::string> passwordHash_;

public:
    UserLogin() = default;
    explicit UserLogin(const std::string& email, const std::optional<std::string>& passwordHash);

    const std::string& getEmail() const noexcept;
    const std::optional<std::string>& getPasswordHash() const noexcept;

protected:
    void setEmail(const std::string& email);
    void setPasswordHash(const std::optional<std::string>& passwordHash);
};
