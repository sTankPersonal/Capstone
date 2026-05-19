#pragma once
#include "infrastructureServices/persistance/BudgetRepository.h"
#include "budget/BudgetGoal.hpp"
#include <cstdint>
#include <string>

class SetBudgetGoal {
    BudgetRepository& repo_;
public:
    explicit SetBudgetGoal(BudgetRepository& repo) : repo_(repo) {}
    BudgetGoal execute(uint32_t userId, double endGoal, double amount, const std::string& description);
};
