#pragma once
#include "IRepository.h"
#include "User.h"
#include "UserId.h"
#include <optional>
#include <string>
#include <utility>

class IUserRepository : public IRepository<User, UserId> {
public:
    virtual std::optional<std::pair<UserId, std::string>> lookupCredentials(const std::string& email) = 0;
};
