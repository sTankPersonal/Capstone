#include "application/users/services/ListTransactionsByCategory.h"
#include <algorithm>

ListTransactionsByCategory::ListTransactionsByCategory(ITransactionRepository& repo) : repo_(repo) {}

std::vector<TransactionDto> ListTransactionsByCategory::execute(const ListTransactionsByCategoryQuery& request) {
    auto all = repo_.findByUserId(request.userId);
    std::vector<TransactionDto> dtos;
    for (const auto& tx : all) {
        if (tx.getCategoryId().getId() == request.categoryId.getId())
            dtos.emplace_back(tx);
    }
    return dtos;
}
