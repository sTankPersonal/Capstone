#pragma once
#include "TransactionId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>
#include <optional>
#include <string>

struct UpdateTransactionCommand {
    TransactionId id;
    TransactionCategoryId categoryId;
    double amount;
    std::optional<CurrencyId> currencyId;
    std::string description;
    std::chrono::year_month_day date;

    explicit UpdateTransactionCommand(TransactionId id, TransactionCategoryId categoryId, double amount,
                             std::optional<CurrencyId> currencyId, std::string description,
                             std::chrono::year_month_day date)
        : id(std::move(id)), categoryId(std::move(categoryId)), amount(amount),
          currencyId(std::move(currencyId)), description(std::move(description)), date(date) {}
};
