#pragma once

#include "ITransactionRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <vector>

class PostgresTransactionRepository : public ITransactionRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresTransactionRepository(DatabaseConnection& db);

    Transaction create(const Transaction& transaction) override;
    std::optional<Transaction> findById(TransactionId id) override;
    std::vector<Transaction> findAll() override;
    bool update(const Transaction& transaction) override;
    bool remove(TransactionId id) override;

    std::vector<Transaction> findByUserId(const UserId& userId) override;
    std::optional<Transaction> findByPlaidTransactionId(const std::string& plaidTransactionId) override;
};
