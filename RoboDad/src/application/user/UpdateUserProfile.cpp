#include "application/user/UpdateUserProfile.h"
#include <chrono>

UpdateUserProfile::UpdateUserProfile(IUserRepository& repo) : repo_(repo) {}

bool UpdateUserProfile::execute(const UserId& userId, const UserInformation& info) {
    auto user = repo_.findById(userId);
    if (!user) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User updated(userId, user->getUserLogin(), info, user->getCreatedAt(), today);
    return repo_.update(updated);
}
