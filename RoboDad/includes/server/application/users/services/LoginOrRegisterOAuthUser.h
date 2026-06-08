#pragma once
#include "IUseCase.h"
#include "OAuthLoginCommand.h"
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "UserProfileDto.h"

class LoginOrRegisterOAuthUser : public IUseCase<OAuthLoginCommand, UserProfileDto> {
    IUserRepository& repo_;
    IPasswordHasher& hasher_;
public:
    LoginOrRegisterOAuthUser(IUserRepository& repo, IPasswordHasher& hasher);
    UserProfileDto execute(const OAuthLoginCommand& cmd) override;
};
