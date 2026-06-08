#include "application/users/services/GetTransaction.h"

GetTransaction::GetTransaction(ITransactionRepository& repo) : repo_(repo) {}

std::optional<TransactionDto> GetTransaction::execute(const GetTransactionQuery& request) {
    auto tx = repo_.findById(request.id);
    if (!tx) return std::nullopt;
    return TransactionDto(*tx);
}
