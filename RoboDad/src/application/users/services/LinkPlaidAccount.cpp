#include "application/users/services/LinkPlaidAccount.h"
#include "ImportPlaidTransactionsCommand.h"
#include "PlaidItem.h"
#include "PlaidItemId.h"
#include "UuidGenerator.h"
#include <chrono>

LinkPlaidAccount::LinkPlaidAccount(IPlaidClient& plaidClient,
                                   IPlaidItemRepository& plaidItemRepo,
                                   ImportPlaidTransactions& importPlaidTransactions)
    : plaidClient_(plaidClient)
    , plaidItemRepo_(plaidItemRepo)
    , importPlaidTransactions_(importPlaidTransactions) {}

std::vector<Transaction> LinkPlaidAccount::execute(const LinkPlaidAccountCommand& request) {
    const std::string accessToken = plaidClient_.exchangePublicToken(request.publicToken);

    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    // Upsert: reuse existing PlaidItem for this user if one exists (handles reconnect).
    // On reconnect the new access_token replaces the old one; cursor resets to "" so
    // the next sync starts fresh, but the duplicate-guard in ImportPlaidTransactions
    // skips any plaid_transaction_id already in the DB.
    PlaidItemId itemId{UuidGenerator::generate()};
    auto existing = plaidItemRepo_.findByUserId(request.userId);
    if (existing) {
        itemId = existing->getId();
        existing->setSyncCursor("");
        PlaidItem updated(itemId, request.userId, accessToken, existing->getCreatedAt(), "");
        plaidItemRepo_.update(updated);
    } else {
        PlaidItem item(itemId, request.userId, accessToken, today);
        plaidItemRepo_.create(item);
    }

    ImportPlaidTransactionsCommand importCmd(request.userId, itemId, accessToken);
    return importPlaidTransactions_.execute(importCmd);
}
