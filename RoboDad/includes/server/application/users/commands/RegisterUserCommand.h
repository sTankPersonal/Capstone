#pragma once
#include "UserInformation.h"
#include <string>

struct RegisterUserCommand {
    std::string email;
    std::string password;
    UserInformation info;

    explicit RegisterUserCommand(std::string email, std::string password, UserInformation info)
        : email(std::move(email)), password(std::move(password)), info(std::move(info)) {}
};
