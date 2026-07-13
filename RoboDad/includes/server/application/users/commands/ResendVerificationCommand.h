#pragma once
#include <string>

struct ResendVerificationCommand {
    std::string email;

    explicit ResendVerificationCommand(std::string email) : email(std::move(email)) {}
};
