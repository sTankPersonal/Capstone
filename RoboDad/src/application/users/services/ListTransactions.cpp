#include "application/users/services/ListTransactions.h"
#include <algorithm>

ListTransactions::ListTransactions(ITransactionRepository& repo) : repo_(repo) {}

std::vector<TransactionDto> ListTransactions::execute(const ListTransactionsQuery& request) {
    auto txs = repo_.findByUserId(request.userId);
    std::vector<TransactionDto> dtos;
    dtos.reserve(txs.size());
    for (const auto& tx : txs) dtos.emplace_back(tx);
    return dtos;
}
