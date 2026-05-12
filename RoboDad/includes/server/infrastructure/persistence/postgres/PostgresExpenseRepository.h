#pragma once
#include "domain/repositories/ExpenseRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresExpenseRepository : public ExpenseRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresExpenseRepository(DatabaseConnection& db);

    Finance create(uint32_t userId, const Finance& finance) override;
    std::optional<Finance> findById(uint32_t id) override;
    std::vector<Finance> findByUserId(uint32_t userId) override;
    bool update(const Finance& finance) override;
    bool remove(uint32_t id) override;
};
