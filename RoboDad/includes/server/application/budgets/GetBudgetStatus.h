#pragma once
#include "infrastructureServices/persistance/BudgetRepository.h"
#include "domain/budget/BudgetGoal.h"
#include <cstdint>
#include <vector>

class GetBudgetStatus {
    BudgetRepository& repo_;
public:
    explicit GetBudgetStatus(BudgetRepository& repo) : repo_(repo) {}
    std::vector<BudgetGoal> execute(uint32_t userId);
};
