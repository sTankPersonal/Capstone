#pragma once
#include "IUseCase.h"
#include "UpdateUserPasswordCommand.h"
#include "IUserRepository.h"
#include "IPasswordHasher.h"

class UpdateUserPassword : public IUseCase<UpdateUserPasswordCommand, bool> {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    UpdateUserPassword(IUserRepository& repo, IPasswordHasher& hasher);
    bool execute(const UpdateUserPasswordCommand& request) override;
};
