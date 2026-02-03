#pragma once
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "UserId.h"
#include <string>

class UpdateUserPassword {
    IUserRepository& repo_;
    IPasswordHasher&        hasher_;
public:
    UpdateUserPassword(IUserRepository& repo, IPasswordHasher& hasher);
    bool execute(const UserId& userId, const std::string& currentPassword, const std::string& newPassword);
};
