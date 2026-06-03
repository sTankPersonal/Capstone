#include "application/users/services/UpdateUserPassword.h"
#include "UserLogin.h"
#include <chrono>

UpdateUserPassword::UpdateUserPassword(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

bool UpdateUserPassword::execute(const UpdateUserPasswordCommand& request) {
    auto user = repo_.findById(request.userId);
    if (!user) return false;
    const auto& stored = user->getUserLogin().getPasswordHash();
    if (!stored || !hasher_.verify(request.currentPassword, *stored)) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User updated(request.userId,
                 UserLogin(user->getUserLogin().getEmail(), hasher_.hash(request.newPassword)),
                 user->getUserInformation(),
                 user->getCreatedAt(),
                 today);
    return repo_.update(updated);
}
