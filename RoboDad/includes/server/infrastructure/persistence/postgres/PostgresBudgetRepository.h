#pragma once
#include "domain/repositories/BudgetRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresBudgetRepository : public BudgetRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresBudgetRepository(DatabaseConnection& db);

    BudgetGoal create(const BudgetGoal& goal) override;
    std::optional<BudgetGoal> findById(uint32_t id) override;
    std::vector<BudgetGoal> findByUserId(uint32_t userId) override;
    bool update(const BudgetGoal& goal) override;
    bool remove(uint32_t id) override;
};
