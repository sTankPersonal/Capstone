#pragma once
#include "IUseCase.h"
#include "LoginUserCommand.h"
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "UserProfileDto.h"
#include <optional>

class LoginUser : public IUseCase<LoginUserCommand, std::optional<UserProfileDto>> {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    LoginUser(IUserRepository& repo, IPasswordHasher& hasher);
    std::optional<UserProfileDto> execute(const LoginUserCommand& request) override;
};
