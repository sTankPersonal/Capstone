#include "application/users/services/VerifyEmail.h"
#include <chrono>

VerifyEmail::VerifyEmail(IUserRepository& repo) : repo_(repo) {}

VerifyEmailResult VerifyEmail::execute(const VerifyEmailCommand& request) {
    auto user = repo_.findByVerificationToken(request.token);
    if (!user) return VerifyEmailResult::NotFound;

    const auto& expiresAt = user->getVerificationTokenExpiresAt();
    if (!expiresAt || *expiresAt < std::chrono::system_clock::now()) {
        return VerifyEmailResult::Expired;
    }

    user->markVerified();
    repo_.update(*user);
    return VerifyEmailResult::Verified;
}
