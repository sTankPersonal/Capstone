#pragma once
#include "IUseCase.h"
#include "GetTransactionQuery.h"
#include "ITransactionRepository.h"
#include "TransactionDto.h"
#include <optional>

class GetTransaction : public IUseCase<GetTransactionQuery, std::optional<TransactionDto>> {
    ITransactionRepository& repo_;
public:
    explicit GetTransaction(ITransactionRepository& repo);
    std::optional<TransactionDto> execute(const GetTransactionQuery& request) override;
};
