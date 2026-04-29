#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "finance/Finance.hpp"

class ExpenseRepository {
public:
    virtual ~ExpenseRepository() = default;
    virtual Finance create(uint32_t userId, const Finance& finance) = 0;
    virtual std::optional<Finance> findById(uint32_t id) = 0;
    virtual std::vector<Finance> findByUserId(uint32_t userId) = 0;
    virtual bool update(const Finance& finance) = 0;
    virtual bool remove(uint32_t id) = 0;
};
