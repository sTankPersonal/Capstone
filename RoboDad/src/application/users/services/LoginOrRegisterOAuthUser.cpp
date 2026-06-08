#include "application/users/services/LoginOrRegisterOAuthUser.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include "UuidGenerator.h"
#include <chrono>

LoginOrRegisterOAuthUser::LoginOrRegisterOAuthUser(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

UserProfileDto LoginOrRegisterOAuthUser::execute(const OAuthLoginCommand& cmd) {
    // If a user with this email already exists, return their profile.
    // Google has already authenticated them so no password check is needed.
    auto creds = repo_.lookupCredentials(cmd.email);
    if (creds) {
        auto user = repo_.findById(creds->first);
        if (user) return UserProfileDto(*user);
    }

    // New OAuth user — store a random unusable password hash so the
    // traditional login path cannot be used with a blank password.
    const std::string randomHash = hasher_.hash(UuidGenerator::generate());
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };

    User newUser(
        UserId(UuidGenerator::generate()),
        UserLogin(cmd.email, randomHash),
        UserInformation(
            cmd.firstName.empty() ? std::nullopt : std::optional<std::string>{cmd.firstName},
            cmd.lastName.empty()  ? std::nullopt : std::optional<std::string>{cmd.lastName},
            std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt
        ),
        today,
        today
    );
    return UserProfileDto(repo_.create(newUser));
}
