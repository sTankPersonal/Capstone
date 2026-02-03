#pragma once
#include "ITransactionRepository.h"
#include "Transactions.h"
#include "TransactionCategoryId.h"
#include "UserId.h"
#include <vector>

class ListTransactionsByCategory {
    ITransactionRepository& repo_;
public:
    explicit ListTransactionsByCategory(ITransactionRepository& repo);
    std::vector<Transaction> execute(const UserId& userId, const TransactionCategoryId& categoryId);
};
