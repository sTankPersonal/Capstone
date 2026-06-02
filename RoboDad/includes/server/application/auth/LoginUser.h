#pragma once
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "User.h"
#include <optional>
#include <string>

class LoginUser {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    LoginUser(IUserRepository& repo, IPasswordHasher& hasher);
    std::optional<User> execute(const std::string& email, const std::string& password);
};
