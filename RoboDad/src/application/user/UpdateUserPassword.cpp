#include "application/user/UpdateUserPassword.h"
#include "UserLogin.h"
#include <chrono>

UpdateUserPassword::UpdateUserPassword(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

bool UpdateUserPassword::execute(const UserId& userId,
                                 const std::string& currentPassword,
                                 const std::string& newPassword) {
    auto user = repo_.findById(userId);
    if (!user) return false;
    const auto& stored = user->getUserLogin().getPasswordHash();
    if (!stored || !hasher_.verify(currentPassword, *stored)) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User updated(userId,
                 UserLogin(user->getUserLogin().getEmail(), hasher_.hash(newPassword)),
                 user->getUserInformation(),
                 user->getCreatedAt(),
                 today);
    return repo_.update(updated);
}
