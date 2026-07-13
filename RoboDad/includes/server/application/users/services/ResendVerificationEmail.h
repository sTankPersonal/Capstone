#pragma once
#include "IUseCase.h"
#include "ResendVerificationCommand.h"
#include "IUserRepository.h"
#include "IEmailService.h"

// Always completes without signalling whether the email exists, so the
// controller cannot leak account existence back to the caller.
class ResendVerificationEmail : public IUseCase<ResendVerificationCommand, void> {
    IUserRepository& repo_;
    IEmailService& emailService_;
public:
    ResendVerificationEmail(IUserRepository& repo, IEmailService& emailService);
    void execute(const ResendVerificationCommand& request) override;
};
