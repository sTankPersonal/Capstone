#pragma once
#include "IRepository.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "UserId.h"
#include <vector>

class ITransactionRepository : public IRepository<Transaction, TransactionId> {
public:
    virtual std::vector<Transaction> findByUserId(const UserId& userId) = 0;
};
