#include "application/auth/RegisterUser.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UuidGenerator.h"
#include <chrono>

RegisterUser::RegisterUser(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

User RegisterUser::execute(const std::string& email,
                           const std::string& password,
                           const UserInformation& info) {
    const std::string hash = hasher_.hash(password);
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User newUser(
        UserId(UuidGenerator::generate()),
        UserLogin(email, hash),
        info,
        today,
        today
    );
    return repo_.create(newUser);
}
