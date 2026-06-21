#pragma once
#include "UserId.h"
#include <string>

struct LinkPlaidAccountCommand {
    UserId userId;
    std::string publicToken;

    explicit LinkPlaidAccountCommand(UserId userId, std::string publicToken)
        : userId(std::move(userId)), publicToken(std::move(publicToken)) {}
};
