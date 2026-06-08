#include "application/references/transactionCategories/services/ListTransactionCategories.h"

ListTransactionCategories::ListTransactionCategories(ITransactionCategoryRepository& repo) : repo_(repo) {}

std::vector<TransactionCategoryDto> ListTransactionCategories::execute(const ListTransactionCategoriesQuery&) {
    auto items = repo_.findAll();
    std::vector<TransactionCategoryDto> dtos;
    dtos.reserve(items.size());
    for (const auto& c : items) dtos.emplace_back(c);
    return dtos;
}
