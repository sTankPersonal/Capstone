#pragma once
#include "UserId.h"
#include <string>

struct UpdateUserPasswordCommand {
    UserId userId;
    std::string currentPassword;
    std::string newPassword;

    explicit UpdateUserPasswordCommand(UserId userId, std::string currentPassword, std::string newPassword)
        : userId(std::move(userId)), currentPassword(std::move(currentPassword)), newPassword(std::move(newPassword)) {}
};
