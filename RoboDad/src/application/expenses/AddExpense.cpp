#include "application/expenses/AddExpense.h"

Finance AddExpense::execute(uint32_t userId, double amount, const std::string& description, FinanceEnum type) {
    Finance expense(amount, description, type);
    return repo_.create(userId, expense);
}
