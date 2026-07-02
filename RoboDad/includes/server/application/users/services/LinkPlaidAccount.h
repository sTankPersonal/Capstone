#pragma once
#include "IUseCase.h"
#include "LinkPlaidAccountCommand.h"
#include "IPlaidClient.h"
#include "IPlaidItemRepository.h"
#include "ImportPlaidTransactions.h"
#include "Transactions.h"
#include <vector>

class LinkPlaidAccount : public IUseCase<LinkPlaidAccountCommand, std::vector<Transaction>> {
    IPlaidClient&            plaidClient_;
    IPlaidItemRepository&    plaidItemRepo_;
    ImportPlaidTransactions& importPlaidTransactions_;
public:
    LinkPlaidAccount(IPlaidClient& plaidClient,
                     IPlaidItemRepository& plaidItemRepo,
                     ImportPlaidTransactions& importPlaidTransactions);

    std::vector<Transaction> execute(const LinkPlaidAccountCommand& request) override;
};
