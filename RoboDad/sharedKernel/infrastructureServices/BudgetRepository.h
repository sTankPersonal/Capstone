#pragma once
#include <cstdint>
#include <vector>
#include "IRepository.h"
#include "budget/BudgetGoal.hpp"

class BudgetRepository : public IRepository<BudgetGoal> {
public:
    virtual BudgetGoal create(const BudgetGoal& goal) = 0;
    virtual std::vector<BudgetGoal> findByUserId(uint32_t userId) = 0;
};
