#pragma once
#include "infrastructureServices/ExpenseRepository.h"
#include "finance/Finance.hpp"
#include <cstdint>
#include <vector>

class ListExpenses {
    ExpenseRepository& repo_;
public:
    explicit ListExpenses(ExpenseRepository& repo) : repo_(repo) {}
    std::vector<Finance> execute(uint32_t userId);
};
