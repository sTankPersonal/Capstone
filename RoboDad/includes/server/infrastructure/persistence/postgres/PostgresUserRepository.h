#pragma once
#include "domain/repositories/UserRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresUserRepository : public UserRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresUserRepository(DatabaseConnection& db);

    User create(const User& user) override;
    std::optional<User> findById(uint32_t id) override;
    std::vector<User> findAll() override;
    bool update(const User& user) override;
    bool remove(uint32_t id) override;
};
