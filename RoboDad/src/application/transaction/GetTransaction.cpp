#include "application/transaction/GetTransaction.h"

GetTransaction::GetTransaction(ITransactionRepository& repo) : repo_(repo) {}

std::optional<Transaction> GetTransaction::execute(const TransactionId& id) {
    return repo_.findById(id);
}
