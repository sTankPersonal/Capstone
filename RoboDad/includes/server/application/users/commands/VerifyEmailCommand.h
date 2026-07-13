#pragma once
#include <string>

struct VerifyEmailCommand {
    std::string token;

    explicit VerifyEmailCommand(std::string token) : token(std::move(token)) {}
};
