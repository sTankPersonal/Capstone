#pragma once
#include "IUseCase.h"
#include "ListTransactionCategoriesQuery.h"
#include "ITransactionCategoryRepository.h"
#include "TransactionCategoryDto.h"
#include <vector>

class ListTransactionCategories : public IUseCase<ListTransactionCategoriesQuery, std::vector<TransactionCategoryDto>> {
    ITransactionCategoryRepository& repo_;
public:
    explicit ListTransactionCategories(ITransactionCategoryRepository& repo);
    std::vector<TransactionCategoryDto> execute(const ListTransactionCategoriesQuery& request) override;
};
