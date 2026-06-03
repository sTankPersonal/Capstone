#include "application/users/services/RegisterUser.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UuidGenerator.h"
#include <chrono>

RegisterUser::RegisterUser(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

UserProfileDto RegisterUser::execute(const RegisterUserCommand& request) {
    const std::string hash = hasher_.hash(request.password);
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User newUser(
        UserId(UuidGenerator::generate()),
        UserLogin(request.email, hash),
        request.info,
        today,
        today
    );
    return UserProfileDto(repo_.create(newUser));
}
