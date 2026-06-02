#pragma once
#include "IUserRepository.h"
#include "User.h"
#include "UserId.h"
#include <optional>

class GetUserProfile {
    IUserRepository& repo_;
public:
    explicit GetUserProfile(IUserRepository& repo);
    std::optional<User> execute(const UserId& userId);
};
