#include "application/users/services/UpdateUserProfile.h"
#include <chrono>

UpdateUserProfile::UpdateUserProfile(IUserRepository& repo) : repo_(repo) {}

bool UpdateUserProfile::execute(const UpdateUserProfileCommand& request) {
    auto user = repo_.findById(request.userId);
    if (!user) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    User updated(request.userId, user->getUserLogin(), request.info, user->getCreatedAt(), today);
    return repo_.update(updated);
}
