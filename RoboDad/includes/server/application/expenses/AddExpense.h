#pragma once
#include "infrastructureServices/persistance/ExpenseRepository.h"
#include "domain/finance/Finance.h"
#include "domain/finance/FinanceEnum.h"
#include <cstdint>
#include <string>

class AddExpense {
    ExpenseRepository& repo_;
public:
    explicit AddExpense(ExpenseRepository& repo) : repo_(repo) {}
    Finance execute(uint32_t userId, double amount, const std::string& description, FinanceEnum type);
};
