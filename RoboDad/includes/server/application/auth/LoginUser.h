#pragma once
#include "infrastructureServices/persistance/UserRepository.h"
#include "user/User.hpp"
#include <cstdint>
#include <optional>

class LoginUser {
    UserRepository& repo_;
public:
    explicit LoginUser(UserRepository& repo) : repo_(repo) {}
    std::optional<User> execute(uint32_t userId);
};
