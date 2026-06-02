#pragma once
#include "ITransactionRepository.h"
#include "Transactions.h"
#include "UserId.h"
#include <vector>

class ListTransactions {
    ITransactionRepository& repo_;
public:
    explicit ListTransactions(ITransactionRepository& repo);
    std::vector<Transaction> execute(const UserId& userId);
};
