#include "application/users/services/UpdateTransaction.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"

UpdateTransaction::UpdateTransaction(ITransactionRepository& repo) : repo_(repo) {}

bool UpdateTransaction::execute(const UpdateTransactionCommand& request) {
    auto tx = repo_.findById(request.id);
    if (!tx) return false;
    Transaction updated(
        request.id,
        tx->getUserId(),
        request.categoryId,
        TransactionAmount(request.amount, request.currencyId),
        TransactionDescription(request.description),
        request.date,
        tx->getCreatedAt(),
        tx->getPlaidTransactionId(),
        request.pfcDetailedCategoryId
    );
    return repo_.update(updated);
}
