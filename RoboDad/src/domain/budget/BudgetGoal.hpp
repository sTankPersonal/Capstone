#pragma once
#include <cstdint>
#include <string>

class BudgetGoal {
    uint32_t id;
    uint32_t userId;
    double endGoal;
    double amount;
    std::string description;

public:
    BudgetGoal(uint32_t id, uint32_t userId, double endGoal, double amount, const std::string& description)
        : id(id), userId(userId), endGoal(endGoal), amount(amount), description(description) {}

    BudgetGoal(double endGoal, double amount, const std::string& description)
        : id(0), userId(0), endGoal(endGoal), amount(amount), description(description) {}

    uint32_t getId() const { return id; }
    uint32_t getUserId() const { return userId; }
    double getEndGoal() const { return endGoal; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }

    void setId(uint32_t newId) { id = newId; }
    void setUserId(uint32_t newUserId) { userId = newUserId; }
    void setEndGoal(double newEndGoal) { endGoal = newEndGoal; }
    void setAmount(double newAmount) { amount = newAmount; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
};