#include "application/users/services/ImportPlaidTransactions.h"
#include "TransactionId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "UuidGenerator.h"
#include <chrono>
#include <sstream>

ImportPlaidTransactions::ImportPlaidTransactions(
    ITransactionRepository&         transactionRepo,
    ITransactionCategoryRepository& categoryRepo,
    ICurrencyRepository&            currencyRepo,
    IPlaidClient&                   plaidClient)
    : transactionRepo_(transactionRepo)
    , categoryRepo_(categoryRepo)
    , currencyRepo_(currencyRepo)
    , plaidClient_(plaidClient) {}

static std::chrono::year_month_day parseDate(const std::string& iso) {
    // Expects "YYYY-MM-DD"
    int y = 0, m = 0, d = 0;
    std::sscanf(iso.c_str(), "%d-%d-%d", &y, &m, &d);
    return std::chrono::year_month_day{
        std::chrono::year{y},
        std::chrono::month{static_cast<unsigned>(m)},
        std::chrono::day{static_cast<unsigned>(d)}
    };
}

std::vector<Transaction> ImportPlaidTransactions::execute(const ImportPlaidTransactionsCommand& request) {
    const std::string accessToken = plaidClient_.createSandboxAccessToken();
    const auto raw = plaidClient_.fetchTransactions(accessToken, request.startDate, request.endDate, request.count);

    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    std::vector<Transaction> created;
    created.reserve(raw.size());

    for (const auto& r : raw) {
        std::string catId = "other";
        if (auto cat = categoryRepo_.findByValue(r.category); cat) {
            catId = cat->getId().getId();
        }

        std::optional<CurrencyId> currencyId;
        if (auto cur = currencyRepo_.findByValue(r.currencyCode); cur) {
            currencyId = cur->getId();
        }

        Transaction tx(
            TransactionId(UuidGenerator::generate()),
            request.userId,
            TransactionCategoryId(catId),
            TransactionAmount(r.amount, currencyId),
            TransactionDescription(r.description),
            parseDate(r.date),
            today
        );
        created.push_back(transactionRepo_.create(tx));
    }

    return created;
}
