#pragma once
#include "IUseCase.h"
#include "RegisterUserCommand.h"
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "IEmailService.h"
#include "UserProfileDto.h"

class RegisterUser : public IUseCase<RegisterUserCommand, UserProfileDto> {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
    IEmailService& emailService_;
public:
    RegisterUser(IUserRepository& repo, IPasswordHasher& hasher, IEmailService& emailService);
    UserProfileDto execute(const RegisterUserCommand& request) override;
};
