#include "application/transaction/UpdateTransaction.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"

UpdateTransaction::UpdateTransaction(ITransactionRepository& repo) : repo_(repo) {}

bool UpdateTransaction::execute(const TransactionId&               id,
                                 const TransactionCategoryId&       categoryId,
                                 double                             amount,
                                 const std::optional<CurrencyId>&   currencyId,
                                 const std::string&                 description,
                                 const std::chrono::year_month_day& date) {
    auto tx = repo_.findById(id);
    if (!tx) return false;
    Transaction updated(
        id,
        tx->getUserId(),
        categoryId,
        TransactionAmount(amount, currencyId),
        TransactionDescription(description),
        date,
        tx->getCreatedAt()
    );
    return repo_.update(updated);
}
