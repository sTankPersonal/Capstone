#include "application/references/transactionCategories/services/GetTransactionCategory.h"

GetTransactionCategory::GetTransactionCategory(ITransactionCategoryRepository& repo) : repo_(repo) {}

std::optional<TransactionCategoryDto> GetTransactionCategory::execute(const GetTransactionCategoryQuery& request) {
    auto category = repo_.findById(request.id);
    if (!category) return std::nullopt;
    return TransactionCategoryDto(*category);
}
