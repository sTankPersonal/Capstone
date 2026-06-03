#pragma once
#include "IUseCase.h"
#include "ListTransactionsByCategoryQuery.h"
#include "ITransactionRepository.h"
#include "TransactionDto.h"
#include <vector>

class ListTransactionsByCategory : public IUseCase<ListTransactionsByCategoryQuery, std::vector<TransactionDto>> {
    ITransactionRepository& repo_;
public:
    explicit ListTransactionsByCategory(ITransactionRepository& repo);
    std::vector<TransactionDto> execute(const ListTransactionsByCategoryQuery& request) override;
};
