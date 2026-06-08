#pragma once
#include "IUseCase.h"
#include "DeleteTransactionCommand.h"
#include "ITransactionRepository.h"

class DeleteTransaction : public IUseCase<DeleteTransactionCommand, bool> {
    ITransactionRepository& repo_;
public:
    explicit DeleteTransaction(ITransactionRepository& repo);
    bool execute(const DeleteTransactionCommand& request) override;
};
