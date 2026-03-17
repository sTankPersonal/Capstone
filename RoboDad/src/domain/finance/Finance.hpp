#pragma once
#include <string>
#include "FinanceEnum.hpp"

class Finance {
    double amount;
    std::string description;
    FinanceEnum type; // Add this line

public:
    Finance(double amt, const std::string& desc, FinanceEnum t)
        : amount(amt), description(desc), type(t) {}

	double getAmount() const { return amount; }
	std::string getDescription() const { return description; }
    FinanceEnum getType() const { return type; }

	void setAmount(double newAmount) { amount = newAmount; }
	void setDescription(const std::string& newDescription) { description = newDescription; }
	void setType(FinanceEnum newType) { type = newType; }
};