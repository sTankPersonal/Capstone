#pragma once
#include "IUseCase.h"
#include "UpdateUserProfileCommand.h"
#include "IUserRepository.h"

class UpdateUserProfile : public IUseCase<UpdateUserProfileCommand, bool> {
    IUserRepository& repo_;
public:
    explicit UpdateUserProfile(IUserRepository& repo);
    bool execute(const UpdateUserProfileCommand& request) override;
};
