#pragma once
#include <cstdint>
#include <vector>
#include "IRepository.h"
#include "finance/Finance.hpp"

class ExpenseRepository : public IRepository<Finance> {
public:
    virtual Finance create(uint32_t userId, const Finance& finance) = 0;
    virtual std::vector<Finance> findByUserId(uint32_t userId) = 0;
};
