#pragma once
#include "IUseCase.h"
#include "DeleteUserCommand.h"
#include "IUserRepository.h"

class DeleteUser : public IUseCase<DeleteUserCommand, bool> {
    IUserRepository& repo_;
public:
    explicit DeleteUser(IUserRepository& repo);
    bool execute(const DeleteUserCommand& request) override;
};
