#pragma once
#include "UserId.h"
#include "UserInformation.h"

struct UpdateUserProfileCommand {
    UserId userId;
    UserInformation info;

    explicit UpdateUserProfileCommand(UserId userId, UserInformation info)
        : userId(std::move(userId)), info(std::move(info)) {}
};
