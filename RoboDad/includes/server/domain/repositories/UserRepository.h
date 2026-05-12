#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "user/User.hpp"

class UserRepository {
public:
    virtual ~UserRepository() = default;
    virtual User create(const User& user) = 0;
    virtual std::optional<User> findById(uint32_t id) = 0;
    virtual std::vector<User> findAll() = 0;
    virtual bool update(const User& user) = 0;
    virtual bool remove(uint32_t id) = 0;
};
