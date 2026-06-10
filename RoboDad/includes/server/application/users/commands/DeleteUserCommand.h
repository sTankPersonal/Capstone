#pragma once
#include "UserId.h"

struct DeleteUserCommand {
    UserId userId;

    explicit DeleteUserCommand(UserId userId)
        : userId(std::move(userId)) {}
};
