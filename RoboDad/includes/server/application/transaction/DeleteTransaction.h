#pragma once
#include "ITransactionRepository.h"
#include "TransactionId.h"

class DeleteTransaction {
    ITransactionRepository& repo_;
public:
    explicit DeleteTransaction(ITransactionRepository& repo);
    bool execute(const TransactionId& id);
};
