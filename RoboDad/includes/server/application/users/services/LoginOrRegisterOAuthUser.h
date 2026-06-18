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

    // Overload that reports whether a brand-new account was created, so callers
    // can redirect first-time OAuth users to finish filling out their profile.
    UserProfileDto execute(const OAuthLoginCommand& cmd, bool& isNewUser);
};
