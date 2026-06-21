#pragma once
#include "IUseCase.h"
#include "ImportPlaidTransactionsCommand.h"
#include "ITransactionRepository.h"
#include "ICurrencyRepository.h"
#include "IPfcDetailedCategoryRepository.h"
#include "IPlaidClient.h"
#include "IPlaidItemRepository.h"
#include "Transactions.h"
#include <vector>

class ImportPlaidTransactions : public IUseCase<ImportPlaidTransactionsCommand, std::vector<Transaction>> {
    ITransactionRepository&        transactionRepo_;
    ICurrencyRepository&           currencyRepo_;
    IPfcDetailedCategoryRepository& pfcDetailedRepo_;
    IPlaidClient&                  plaidClient_;
    IPlaidItemRepository&          plaidItemRepo_;
public:
    ImportPlaidTransactions(ITransactionRepository&        transactionRepo,
                            ICurrencyRepository&           currencyRepo,
                            IPfcDetailedCategoryRepository& pfcDetailedRepo,
                            IPlaidClient&                  plaidClient,
                            IPlaidItemRepository&          plaidItemRepo);

    std::vector<Transaction> execute(const ImportPlaidTransactionsCommand& request) override;
};
