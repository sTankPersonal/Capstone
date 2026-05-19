#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <utility>
#include <vector>
#include "IRepository.h"
#include "domain/user/User.h"

class UserRepository : public IRepository<User> {
public:
    virtual User create(const User& user, const std::string& email, const std::string& passwordHash) = 0;
    virtual std::vector<User> findAll() = 0;
    virtual std::optional<std::pair<uint32_t, std::string>> lookupCredentials(const std::string& email) = 0;
};
