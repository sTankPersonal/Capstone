#pragma once
#include "IUseCase.h"
#include "ListTransactionsQuery.h"
#include "ITransactionRepository.h"
#include "TransactionDto.h"
#include <vector>

class ListTransactions : public IUseCase<ListTransactionsQuery, std::vector<TransactionDto>> {
    ITransactionRepository& repo_;
public:
    explicit ListTransactions(ITransactionRepository& repo);
    std::vector<TransactionDto> execute(const ListTransactionsQuery& request) override;
};
