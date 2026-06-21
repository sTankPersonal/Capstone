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
    ITransactionRepository& transactionRepo,
    ICurrencyRepository&    currencyRepo,
    IPlaidClient&           plaidClient,
    IPlaidItemRepository&   plaidItemRepo)
    : transactionRepo_(transactionRepo)
    , currencyRepo_(currencyRepo)
    , plaidClient_(plaidClient)
    , plaidItemRepo_(plaidItemRepo) {}

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

static std::optional<CurrencyId> resolveCurrency(ICurrencyRepository& repo, const std::string& code) {
    if (auto cur = repo.findByValue(code); cur)
        return cur->getId();
    return std::nullopt;
}

std::vector<Transaction> ImportPlaidTransactions::execute(const ImportPlaidTransactionsCommand& request) {
    const auto syncResult = plaidClient_.fetchTransactions(request.accessToken, request.cursor);

    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    std::vector<Transaction> created;

    // ── added ────────────────────────────────────────────────────────────────
    for (const auto& r : syncResult.added) {
        // Skip if already imported (handles reconnect with reset cursor).
        if (!r.plaidTransactionId.empty() &&
            transactionRepo_.findByPlaidTransactionId(r.plaidTransactionId))
            continue;

        std::string catId = (r.amount >= 0.0) ? "expenses" : "earnings";
        Transaction tx(
            TransactionId(UuidGenerator::generate()),
            request.userId,
            TransactionCategoryId(catId),
            TransactionAmount(std::abs(r.amount), resolveCurrency(currencyRepo_, r.currencyCode)),
            TransactionDescription(r.description),
            parseDate(r.date),
            today,
            r.plaidTransactionId.empty() ? std::nullopt : std::make_optional(r.plaidTransactionId)
        );
        created.push_back(transactionRepo_.create(tx));
    }

    // ── modified ─────────────────────────────────────────────────────────────
    for (const auto& r : syncResult.modified) {
        if (r.plaidTransactionId.empty()) continue;
        auto existing = transactionRepo_.findByPlaidTransactionId(r.plaidTransactionId);
        if (!existing) continue;

        std::string catId = (r.amount >= 0.0) ? "expenses" : "earnings";
        Transaction updated(
            existing->getId(),
            existing->getUserId(),
            TransactionCategoryId(catId),
            TransactionAmount(std::abs(r.amount), resolveCurrency(currencyRepo_, r.currencyCode)),
            TransactionDescription(r.description),
            parseDate(r.date),
            existing->getCreatedAt(),
            r.plaidTransactionId
        );
        transactionRepo_.update(updated);
    }

    // ── removed ──────────────────────────────────────────────────────────────
    for (const auto& plaidTxId : syncResult.removed) {
        auto existing = transactionRepo_.findByPlaidTransactionId(plaidTxId);
        if (existing)
            transactionRepo_.remove(existing->getId());
    }

    // ── persist cursor ───────────────────────────────────────────────────────
    auto item = plaidItemRepo_.findById(request.plaidItemId);
    if (!syncResult.nextCursor.empty() && item) {
        item->setSyncCursor(syncResult.nextCursor);
        plaidItemRepo_.update(*item);
    }

    return created;
}
