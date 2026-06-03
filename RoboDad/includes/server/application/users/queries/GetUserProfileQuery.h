#pragma once
#include "UserId.h"

struct GetUserProfileQuery {
    UserId userId;

    explicit GetUserProfileQuery(UserId userId)
        : userId(std::move(userId)) {}
};
