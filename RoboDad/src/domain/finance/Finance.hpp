#pragma once
#include <string>
#include "FinanceEnum.hpp"

class Finance {
    uint32_t id;
    double amount;
    std::string description;
    FinanceEnum type;

public:
    Finance(uint32_t id, double amt, const std::string& desc, FinanceEnum t)
        : id(id), amount(amt), description(desc), type(t) {}

    Finance(double amt, const std::string& desc, FinanceEnum t)
        : id(0), amount(amt), description(desc), type(t) {}

    uint32_t getId() const { return id; }
    double getAmount() const { return amount; }
    std::string getDescription() const { return description; }
    FinanceEnum getType() const { return type; }

    void setId(uint32_t newId) { id = newId; }
    void setAmount(double newAmount) { amount = newAmount; }
    void setDescription(const std::string& newDescription) { description = newDescription; }
    void setType(FinanceEnum newType) { type = newType; }
};