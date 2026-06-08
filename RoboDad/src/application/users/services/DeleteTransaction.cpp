#include "application/users/services/DeleteTransaction.h"

DeleteTransaction::DeleteTransaction(ITransactionRepository& repo) : repo_(repo) {}

bool DeleteTransaction::execute(const DeleteTransactionCommand& request) {
    return repo_.remove(request.id);
}
