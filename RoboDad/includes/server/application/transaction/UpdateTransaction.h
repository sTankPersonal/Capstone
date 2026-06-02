#pragma once
#include "ITransactionRepository.h"
#include "TransactionId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>
#include <optional>
#include <string>

class UpdateTransaction {
    ITransactionRepository& repo_;
public:
    explicit UpdateTransaction(ITransactionRepository& repo);
    bool execute(const TransactionId&                    id,
                 const TransactionCategoryId&            categoryId,
                 double                                  amount,
                 const std::optional<CurrencyId>&        currencyId,
                 const std::string&                      description,
                 const std::chrono::year_month_day&      date);
};
