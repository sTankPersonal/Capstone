#pragma once
#include "infrastructureServices/persistance/UserRepository.h"
#include "infrastructureServices/security/IPasswordHasher.h"
#include "domain/user/User.h"
#include "domain/user/PersonalInfo.h"
#include <string>

class RegisterUser {
    UserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    RegisterUser(UserRepository& repo, IPasswordHasher& hasher) : repo_(repo), hasher_(hasher) {}
    User execute(const PersonalInfo& info, const std::string& email, const std::string& password);
};
