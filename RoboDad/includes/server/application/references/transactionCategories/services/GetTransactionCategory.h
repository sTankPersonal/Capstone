#pragma once
#include "IUseCase.h"
#include "GetTransactionCategoryQuery.h"
#include "ITransactionCategoryRepository.h"
#include "TransactionCategoryDto.h"
#include <optional>

class GetTransactionCategory : public IUseCase<GetTransactionCategoryQuery, std::optional<TransactionCategoryDto>> {
    ITransactionCategoryRepository& repo_;
public:
    explicit GetTransactionCategory(ITransactionCategoryRepository& repo);
    std::optional<TransactionCategoryDto> execute(const GetTransactionCategoryQuery& request) override;
};
