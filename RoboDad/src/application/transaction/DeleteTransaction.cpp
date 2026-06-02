#include "application/transaction/DeleteTransaction.h"

DeleteTransaction::DeleteTransaction(ITransactionRepository& repo) : repo_(repo) {}

bool DeleteTransaction::execute(const TransactionId& id) {
    return repo_.remove(id);
}
