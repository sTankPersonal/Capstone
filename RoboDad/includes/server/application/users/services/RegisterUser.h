#pragma once
#include "IUseCase.h"
#include "RegisterUserCommand.h"
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "UserProfileDto.h"

class RegisterUser : public IUseCase<RegisterUserCommand, UserProfileDto> {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    RegisterUser(IUserRepository& repo, IPasswordHasher& hasher);
    UserProfileDto execute(const RegisterUserCommand& request) override;
};
