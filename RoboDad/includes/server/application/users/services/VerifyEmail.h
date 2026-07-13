#pragma once
#include "IUseCase.h"
#include "VerifyEmailCommand.h"
#include "IUserRepository.h"

enum class VerifyEmailResult {
    Verified,
    Expired,
    NotFound
};

class VerifyEmail : public IUseCase<VerifyEmailCommand, VerifyEmailResult> {
    IUserRepository& repo_;
public:
    explicit VerifyEmail(IUserRepository& repo);
    VerifyEmailResult execute(const VerifyEmailCommand& request) override;
};
