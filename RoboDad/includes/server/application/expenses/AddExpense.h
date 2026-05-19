#pragma once
#include "infrastructureServices/ExpenseRepository.h"
#include "finance/Finance.hpp"
#include "finance/FinanceEnum.hpp"
#include <cstdint>
#include <string>

class AddExpense {
    ExpenseRepository& repo_;
public:
    explicit AddExpense(ExpenseRepository& repo) : repo_(repo) {}
    Finance execute(uint32_t userId, double amount, const std::string& description, FinanceEnum type);
};
