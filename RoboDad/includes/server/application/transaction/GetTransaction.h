#pragma once
#include "ITransactionRepository.h"
#include "Transactions.h"
#include "TransactionId.h"
#include <optional>

class GetTransaction {
    ITransactionRepository& repo_;
public:
    explicit GetTransaction(ITransactionRepository& repo);
    std::optional<Transaction> execute(const TransactionId& id);
};
