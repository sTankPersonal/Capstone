#include "TransactionCategory.h"

TransactionCategory::TransactionCategory(const TransactionCategoryId& id, const std::string& value, const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const TransactionCategoryId& TransactionCategory::getId() const noexcept { return id_; }
const std::string& TransactionCategory::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& TransactionCategory::getCreatedAt() const noexcept { return createdAt_; }

void TransactionCategory::setValue(const std::string& value) { value_ = value; }
