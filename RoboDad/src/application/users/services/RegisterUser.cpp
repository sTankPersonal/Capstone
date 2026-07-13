#include "application/users/services/RegisterUser.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UuidGenerator.h"
#include <chrono>

RegisterUser::RegisterUser(IUserRepository& repo, IPasswordHasher& hasher, IEmailService& emailService)
    : repo_(repo), hasher_(hasher), emailService_(emailService) {}

UserProfileDto RegisterUser::execute(const RegisterUserCommand& request) {
    const std::string hash = hasher_.hash(request.password);
    const auto now = std::chrono::system_clock::now();
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(now)
    };
    const std::string verificationToken = UuidGenerator::generate();
    const auto verificationTokenExpiresAt = now + std::chrono::hours(24);

    User newUser(
        UserId(UuidGenerator::generate()),
        UserLogin(request.email, hash),
        request.info,
        today,
        today,
        false,
        verificationToken,
        verificationTokenExpiresAt
    );
    UserProfileDto profile(repo_.create(newUser));
    emailService_.sendVerificationEmail(request.email, verificationToken);
    return profile;
}
