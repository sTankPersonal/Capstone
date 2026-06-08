#include "application/users/services/CreateTransaction.h"
#include "TransactionId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "UuidGenerator.h"
#include <chrono>

CreateTransaction::CreateTransaction(ITransactionRepository& repo) : repo_(repo) {}

TransactionDto CreateTransaction::execute(const CreateTransactionCommand& request) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    Transaction tx(
        TransactionId(UuidGenerator::generate()),
        request.userId,
        request.categoryId,
        TransactionAmount(request.amount, request.currencyId),
        TransactionDescription(request.description),
        request.date,
        today
    );
    return TransactionDto(repo_.create(tx));
}
