#pragma once
#include <cstdint>
#include <vector>
#include "IRepository.h"
#include "user/User.hpp"

class UserRepository : public IRepository<User> {
public:
    virtual User create(const User& user) = 0;
    virtual std::vector<User> findAll() = 0;
};
