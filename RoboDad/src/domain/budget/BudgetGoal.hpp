#pragma once
#include <string>

class BudgetGoal {
	double endGoal;
	double amount;
	std::string description;

public:
	BudgetGoal(double endGoal, double amount, const std::string& description)
		: endGoal(endGoal), amount(amount), description(description) {
	}
	double getEndGoal() const { return endGoal; }
	double getAmount() const { return amount; }
	std::string getDescription() const { return description; }

	void setEndGoal(double newEndGoal) { endGoal = newEndGoal; }
	void setAmount(double newAmount) { amount = newAmount; }
	void setDescription(const std::string& newDescription) { description = newDescription; }
};