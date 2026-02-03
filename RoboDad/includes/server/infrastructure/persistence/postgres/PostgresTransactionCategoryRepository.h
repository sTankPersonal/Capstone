#pragma once

#include "ITransactionCategoryRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresTransactionCategoryRepository : public ITransactionCategoryRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresTransactionCategoryRepository(DatabaseConnection& db);

    TransactionCategory create(const TransactionCategory& entity) override;
    std::optional<TransactionCategory> findById(TransactionCategoryId id) override;
    std::vector<TransactionCategory> findAll() override;
    bool update(const TransactionCategory& entity) override;
    bool remove(TransactionCategoryId id) override;

    std::optional<TransactionCategory> findByValue(const std::string& value);
};
