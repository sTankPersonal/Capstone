#include "domain/budget/BudgetGoal.h"

BudgetGoal::BudgetGoal(uint32_t id, uint32_t userId, double endGoal, double amount, const std::string& description)
    : id(id), userId(userId), endGoal(endGoal), amount(amount), description(description) {}

BudgetGoal::BudgetGoal(double endGoal, double amount, const std::string& description)
    : id(0), userId(0), endGoal(endGoal), amount(amount), description(description) {}

uint32_t BudgetGoal::getId() const { return id; }
uint32_t BudgetGoal::getUserId() const { return userId; }
double BudgetGoal::getEndGoal() const { return endGoal; }
double BudgetGoal::getAmount() const { return amount; }
std::string BudgetGoal::getDescription() const { return description; }

void BudgetGoal::setId(uint32_t newId) { id = newId; }
void BudgetGoal::setUserId(uint32_t newUserId) { userId = newUserId; }
void BudgetGoal::setEndGoal(double newEndGoal) { endGoal = newEndGoal; }
void BudgetGoal::setAmount(double newAmount) { amount = newAmount; }
void BudgetGoal::setDescription(const std::string& newDescription) { description = newDescription; }
