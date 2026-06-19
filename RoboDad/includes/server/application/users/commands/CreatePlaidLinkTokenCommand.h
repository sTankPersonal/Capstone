#pragma once
#include "UserId.h"

struct CreatePlaidLinkTokenCommand {
    UserId userId;
    explicit CreatePlaidLinkTokenCommand(UserId userId) : userId(std::move(userId)) {}
};
