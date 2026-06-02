#include "application/transaction/ListTransactionsByCategory.h"
#include <algorithm>

ListTransactionsByCategory::ListTransactionsByCategory(ITransactionRepository& repo) : repo_(repo) {}

std::vector<Transaction> ListTransactionsByCategory::execute(const UserId& userId,
                                                             const TransactionCategoryId& categoryId) {
    auto all = repo_.findByUserId(userId);
    std::vector<Transaction> filtered;
    std::copy_if(all.begin(), all.end(), std::back_inserter(filtered),
        [&](const Transaction& tx) {
            return tx.getCategoryId().getId() == categoryId.getId();
        });
    return filtered;
}
