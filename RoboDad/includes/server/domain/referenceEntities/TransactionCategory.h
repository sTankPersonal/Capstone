#pragma once

#include "IEntity.h"
#include "TransactionCategoryId.h"
#include <chrono>
#include <string>

class TransactionCategory : public IEntity<TransactionCategoryId, std::chrono::year_month_day>
{
    TransactionCategoryId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit TransactionCategory(const TransactionCategoryId& id, const std::string& value, const std::chrono::year_month_day& createdAt);

    const TransactionCategoryId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setValue(const std::string& value);
};
