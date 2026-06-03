#pragma once
#include "IUseCase.h"
#include "UpdateTransactionCommand.h"
#include "ITransactionRepository.h"

class UpdateTransaction : public IUseCase<UpdateTransactionCommand, bool> {
    ITransactionRepository& repo_;
public:
    explicit UpdateTransaction(ITransactionRepository& repo);
    bool execute(const UpdateTransactionCommand& request) override;
};
