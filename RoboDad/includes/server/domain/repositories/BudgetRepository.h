#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "budget/BudgetGoal.hpp"

class BudgetRepository {
public:
    virtual ~BudgetRepository() = default;
    virtual BudgetGoal create(const BudgetGoal& goal) = 0;
    virtual std::optional<BudgetGoal> findById(uint32_t id) = 0;
    virtual std::vector<BudgetGoal> findByUserId(uint32_t userId) = 0;
    virtual bool update(const BudgetGoal& goal) = 0;
    virtual bool remove(uint32_t id) = 0;
};
