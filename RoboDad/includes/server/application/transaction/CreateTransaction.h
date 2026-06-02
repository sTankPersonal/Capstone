#pragma once
#include "ITransactionRepository.h"
#include "Transactions.h"
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>
#include <optional>
#include <string>

class CreateTransaction {
    ITransactionRepository& repo_;
public:
    explicit CreateTransaction(ITransactionRepository& repo);
    Transaction execute(const UserId&                          userId,
                        const TransactionCategoryId&           categoryId,
                        double                                 amount,
                        const std::optional<CurrencyId>&       currencyId,
                        const std::string&                     description,
                        const std::chrono::year_month_day&     date);
};
