#pragma once
#include "IUseCase.h"
#include "CreateTransactionCommand.h"
#include "ITransactionRepository.h"
#include "TransactionDto.h"

class CreateTransaction : public IUseCase<CreateTransactionCommand, TransactionDto> {
    ITransactionRepository& repo_;
public:
    explicit CreateTransaction(ITransactionRepository& repo);
    TransactionDto execute(const CreateTransactionCommand& request) override;
};
