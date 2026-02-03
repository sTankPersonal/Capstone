#include "application/transaction/ListTransactions.h"

ListTransactions::ListTransactions(ITransactionRepository& repo) : repo_(repo) {}

std::vector<Transaction> ListTransactions::execute(const UserId& userId) {
    return repo_.findByUserId(userId);
}
