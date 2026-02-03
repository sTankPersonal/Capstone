#include "application/user/GetUserProfile.h"

GetUserProfile::GetUserProfile(IUserRepository& repo) : repo_(repo) {}

std::optional<User> GetUserProfile::execute(const UserId& userId) {
    return repo_.findById(userId);
}
