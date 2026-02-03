#pragma once
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "User.h"
#include "UserInformation.h"
#include <string>

class RegisterUser {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    RegisterUser(IUserRepository& repo, IPasswordHasher& hasher);
    User execute(const std::string& email, const std::string& password, const UserInformation& info);
};
