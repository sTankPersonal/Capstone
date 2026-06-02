#pragma once
#include "IUserRepository.h"
#include "UserId.h"
#include "UserInformation.h"

class UpdateUserProfile {
    IUserRepository& repo_;
public:
    explicit UpdateUserProfile(IUserRepository& repo);
    bool execute(const UserId& userId, const UserInformation& info);
};
