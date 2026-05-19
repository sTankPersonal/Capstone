#pragma once
#include "domain/IAggregateRoot.h"
#include <cstdint>
#include <string>

class BudgetGoal : public IAggregateRoot<uint32_t> {
    uint32_t id;
    uint32_t userId;
    double endGoal;
    double amount;
    std::string description;

public:
    BudgetGoal(uint32_t id, uint32_t userId, double endGoal, double amount, const std::string& description);
    BudgetGoal(double endGoal, double amount, const std::string& description);

    uint32_t getId() const override;
    uint32_t getUserId() const;
    double getEndGoal() const;
    double getAmount() const;
    std::string getDescription() const;

    void setId(uint32_t newId);
    void setUserId(uint32_t newUserId);
    void setEndGoal(double newEndGoal);
    void setAmount(double newAmount);
    void setDescription(const std::string& newDescription);
};
