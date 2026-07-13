#include "application/users/services/ResendVerificationEmail.h"
#include "UuidGenerator.h"
#include <chrono>

ResendVerificationEmail::ResendVerificationEmail(IUserRepository& repo, IEmailService& emailService)
    : repo_(repo), emailService_(emailService) {}

void ResendVerificationEmail::execute(const ResendVerificationCommand& request) {
    auto creds = repo_.lookupCredentials(request.email);
    if (!creds) return;

    auto user = repo_.findById(creds->first);
    if (!user || user->isVerified()) return;

    const std::string token = UuidGenerator::generate();
    const auto expiresAt = std::chrono::system_clock::now() + std::chrono::hours(24);
    user->setVerificationToken(token, expiresAt);
    repo_.update(*user);
    emailService_.sendVerificationEmail(request.email, token);
}
