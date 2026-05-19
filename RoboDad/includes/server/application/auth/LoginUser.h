#pragma once
#include "infrastructureServices/persistance/UserRepository.h"
#include "infrastructureServices/security/IPasswordHasher.h"
#include "domain/user/User.h"
#include <optional>
#include <string>

class LoginUser {
    UserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    LoginUser(UserRepository& repo, IPasswordHasher& hasher) : repo_(repo), hasher_(hasher) {}
    std::optional<User> execute(const std::string& email, const std::string& password);
};
