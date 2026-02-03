#pragma once
#include "ITransactionRepository.h"
#include "ITransactionCategoryRepository.h"
#include "ICurrencyRepository.h"
#include "IPlaidClient.h"
#include "Transactions.h"
#include "UserId.h"
#include <string>
#include <vector>

class ImportPlaidTransactions {
    ITransactionRepository&         transactionRepo_;
    ITransactionCategoryRepository& categoryRepo_;
    ICurrencyRepository&            currencyRepo_;
    IPlaidClient&                   plaidClient_;
public:
    ImportPlaidTransactions(ITransactionRepository&         transactionRepo,
                            ITransactionCategoryRepository& categoryRepo,
                            ICurrencyRepository&            currencyRepo,
                            IPlaidClient&                   plaidClient);

    // Fetches sandbox transactions from Plaid, maps them to Transaction entities,
    // persists each, and returns the full list of created Transactions.
    std::vector<Transaction> execute(const UserId&      userId,
                                     const std::string& startDate,
                                     const std::string& endDate,
                                     int                count = 100);
};
