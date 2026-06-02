#pragma once

#include "IUserRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <utility>
#include <vector>

class PostgresUserRepository : public IUserRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresUserRepository(DatabaseConnection& db);

    User create(const User& user) override;
    std::optional<User> findById(UserId id) override;
    std::vector<User> findAll() override;
    bool update(const User& user) override;
    bool remove(UserId id) override;

    // Returns {userId, passwordHash} for the given email, or nullopt if not found.
    std::optional<std::pair<UserId, std::string>> lookupCredentials(const std::string& email);
};
