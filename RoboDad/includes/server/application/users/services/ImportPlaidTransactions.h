#pragma once
#include "IUseCase.h"
#include "ImportPlaidTransactionsCommand.h"
#include "ITransactionRepository.h"
#include "ITransactionCategoryRepository.h"
#include "ICurrencyRepository.h"
#include "IPlaidClient.h"
#include "Transactions.h"
#include <vector>

class ImportPlaidTransactions : public IUseCase<ImportPlaidTransactionsCommand, std::vector<Transaction>> {
    ITransactionRepository&         transactionRepo_;
    ITransactionCategoryRepository& categoryRepo_;
    ICurrencyRepository&            currencyRepo_;
    IPlaidClient&                   plaidClient_;
public:
    ImportPlaidTransactions(ITransactionRepository&         transactionRepo,
                            ITransactionCategoryRepository& categoryRepo,
                            ICurrencyRepository&            currencyRepo,
                            IPlaidClient&                   plaidClient);

    std::vector<Transaction> execute(const ImportPlaidTransactionsCommand& request) override;
};
