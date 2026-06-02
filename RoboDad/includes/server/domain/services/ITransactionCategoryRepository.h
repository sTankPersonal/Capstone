#pragma once
#include "IRepository.h"
#include "TransactionCategory.h"
#include "TransactionCategoryId.h"
#include <optional>
#include <string>

class ITransactionCategoryRepository : public IRepository<TransactionCategory, TransactionCategoryId> {
public:
    virtual std::optional<TransactionCategory> findByValue(const std::string& value) = 0;
};
