#include "application/users/services/GetUserProfile.h"

GetUserProfile::GetUserProfile(IUserRepository& repo) : repo_(repo) {}

std::optional<UserProfileDto> GetUserProfile::execute(const GetUserProfileQuery& request) {
    auto user = repo_.findById(request.userId);
    if (!user) return std::nullopt;
    return UserProfileDto(*user);
}
