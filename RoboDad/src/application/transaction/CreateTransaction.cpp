#include "application/transaction/CreateTransaction.h"
#include "TransactionId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "UuidGenerator.h"
#include <chrono>

CreateTransaction::CreateTransaction(ITransactionRepository& repo) : repo_(repo) {}

Transaction CreateTransaction::execute(const UserId&                      userId,
                                        const TransactionCategoryId&        categoryId,
                                        double                              amount,
                                        const std::optional<CurrencyId>&    currencyId,
                                        const std::string&                  description,
                                        const std::chrono::year_month_day&  date) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    Transaction tx(
        TransactionId(UuidGenerator::generate()),
        userId,
        categoryId,
        TransactionAmount(amount, currencyId),
        TransactionDescription(description),
        date,
        today
    );
    return repo_.create(tx);
}
