#include "application/budgets/SetBudgetGoal.h"

BudgetGoal SetBudgetGoal::execute(uint32_t userId, double endGoal, double amount, const std::string& description) {
    BudgetGoal goal(endGoal, amount, description);
    goal.setUserId(userId);
    return repo_.create(goal);
}
