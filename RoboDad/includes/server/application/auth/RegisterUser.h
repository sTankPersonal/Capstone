#pragma once
#include "infrastructureServices/persistance/UserRepository.h"
#include "domain/user/User.h"
#include "domain/user/PersonalInfo.h"

class RegisterUser {
    UserRepository& repo_;
public:
    explicit RegisterUser(UserRepository& repo) : repo_(repo) {}
    User execute(const PersonalInfo& info);
};
