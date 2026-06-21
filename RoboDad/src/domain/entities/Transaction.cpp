#include "Transactions.h"

Transaction::Transaction(
    const TransactionId& id,
    const UserId& userId,
    const TransactionCategoryId& categoryId,
    const TransactionAmount& amount,
    const TransactionDescription& description,
    const std::chrono::year_month_day& transactionDate,
    const std::chrono::year_month_day& createdAt,
    std::optional<std::string> plaidTransactionId
) : id_(id), userId_(userId), categoryId_(categoryId), amount_(amount), description_(description),
    transactionDate_(transactionDate), createdAt_(createdAt), plaidTransactionId_(std::move(plaidTransactionId)) {}

const TransactionId& Transaction::getId() const noexcept { return id_; }
const UserId& Transaction::getUserId() const noexcept { return userId_; }
const TransactionCategoryId& Transaction::getCategoryId() const noexcept { return categoryId_; }
const TransactionAmount& Transaction::getAmount() const noexcept { return amount_; }
const TransactionDescription& Transaction::getDescription() const noexcept { return description_; }
const std::chrono::year_month_day& Transaction::getTransactionDate() const noexcept { return transactionDate_; }
const std::chrono::year_month_day& Transaction::getCreatedAt() const noexcept { return createdAt_; }
const std::optional<std::string>& Transaction::getPlaidTransactionId() const noexcept { return plaidTransactionId_; }

void Transaction::setCategoryId(const TransactionCategoryId& categoryId) { categoryId_ = categoryId; }
void Transaction::setAmount(const TransactionAmount& amount) { amount_ = amount; }
void Transaction::setDescription(const TransactionDescription& description) { description_ = description; }
void Transaction::setTransactionDate(const std::chrono::year_month_day& transactionDate) { transactionDate_ = transactionDate; }
