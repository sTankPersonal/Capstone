#pragma once
#include "infrastructureServices/BudgetRepository.h"
#include "budget/BudgetGoal.hpp"
#include <cstdint>
#include <vector>

class GetBudgetStatus {
    BudgetRepository& repo_;
public:
    explicit GetBudgetStatus(BudgetRepository& repo) : repo_(repo) {}
    std::vector<BudgetGoal> execute(uint32_t userId);
};
