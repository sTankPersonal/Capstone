#pragma once
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>
#include <optional>
#include <string>

struct CreateTransactionCommand {
    UserId userId;
    TransactionCategoryId categoryId;
    double amount;
    std::optional<CurrencyId> currencyId;
    std::string description;
    std::chrono::year_month_day date;

    explicit CreateTransactionCommand(UserId userId, TransactionCategoryId categoryId, double amount,
                             std::optional<CurrencyId> currencyId, std::string description,
                             std::chrono::year_month_day date)
        : userId(std::move(userId)), categoryId(std::move(categoryId)), amount(amount),
          currencyId(std::move(currencyId)), description(std::move(description)), date(date) {}
};
