#pragma once
#include <string>

struct LoginUserCommand {
    std::string email;
    std::string password;

    explicit LoginUserCommand(const std::string& email, const std::string& password)
        : email(email), password(password) {}
};
