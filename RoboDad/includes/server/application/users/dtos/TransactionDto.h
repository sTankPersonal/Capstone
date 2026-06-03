#pragma once
#include "IDto.h"
#include "Transactions.h"
#include <string>
#include <optional>
#include "crow.h"

class TransactionDto : public IDto {
    std::string id_;
    std::string userId_;
    std::string categoryId_;
    std::optional<double> amount_;
    std::optional<std::string> currencyId_;
    std::optional<std::string> description_;
    std::string transactionDate_;
    std::string createdAt_;

public:
    explicit TransactionDto(const Transaction& transaction);
    explicit operator crow::json::wvalue() const override;

    std::string getUserId() const;
    std::string getCategoryId() const;
};
