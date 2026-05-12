#include "application/budgets/GetBudgetStatus.h"

std::vector<BudgetGoal> GetBudgetStatus::execute(uint32_t userId) {
    return repo_.findByUserId(userId);
}
