#pragma once
#include "domain/repositories/UserRepository.h"
#include "user/User.hpp"
#include "user/PersonalInfo.hpp"

class RegisterUser {
    UserRepository& repo_;
public:
    explicit RegisterUser(UserRepository& repo) : repo_(repo) {}
    User execute(const PersonalInfo& info);
};
