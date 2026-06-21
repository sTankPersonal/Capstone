#pragma once
#include "IUseCase.h"
#include "ImportPlaidTransactionsCommand.h"
#include "ITransactionRepository.h"
#include "ICurrencyRepository.h"
#include "IPlaidClient.h"
#include "IPlaidItemRepository.h"
#include "Transactions.h"
#include <vector>

class ImportPlaidTransactions : public IUseCase<ImportPlaidTransactionsCommand, std::vector<Transaction>> {
    ITransactionRepository& transactionRepo_;
    ICurrencyRepository&    currencyRepo_;
    IPlaidClient&           plaidClient_;
    IPlaidItemRepository&   plaidItemRepo_;
public:
    ImportPlaidTransactions(ITransactionRepository& transactionRepo,
                            ICurrencyRepository&    currencyRepo,
                            IPlaidClient&           plaidClient,
                            IPlaidItemRepository&   plaidItemRepo);

    std::vector<Transaction> execute(const ImportPlaidTransactionsCommand& request) override;
};
