#pragma once

#include "IEntity.h"
#include "TransactionId.h"
#include "TransactionCategoryId.h"
#include "UserId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include <chrono>
#include <optional>
#include <string>

class Transaction : public IEntity<TransactionId, std::chrono::year_month_day>
{
    TransactionId id_;
    UserId userId_;
    TransactionCategoryId categoryId_;
    TransactionAmount amount_;
    TransactionDescription description_;
    std::chrono::year_month_day transactionDate_;
    std::chrono::year_month_day createdAt_;
    std::optional<std::string> plaidTransactionId_;

public:
    explicit Transaction(
        const TransactionId& id,
        const UserId& userId,
        const TransactionCategoryId& categoryId,
        const TransactionAmount& amount,
        const TransactionDescription& description,
        const std::chrono::year_month_day& transactionDate,
        const std::chrono::year_month_day& createdAt,
        std::optional<std::string> plaidTransactionId = std::nullopt
    );

    const TransactionId& getId() const noexcept override;
    const UserId& getUserId() const noexcept;
    const TransactionCategoryId& getCategoryId() const noexcept;
    const TransactionAmount& getAmount() const noexcept;
    const TransactionDescription& getDescription() const noexcept;
    const std::chrono::year_month_day& getTransactionDate() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
    const std::optional<std::string>& getPlaidTransactionId() const noexcept;

protected:
    void setCategoryId(const TransactionCategoryId& categoryId);
    void setAmount(const TransactionAmount& amount);
    void setDescription(const TransactionDescription& description);
    void setTransactionDate(const std::chrono::year_month_day& transactionDate);
};
