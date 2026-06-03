#include "application/users/services/LoginUser.h"

LoginUser::LoginUser(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

std::optional<UserProfileDto> LoginUser::execute(const LoginUserCommand& request) {
    auto creds = repo_.lookupCredentials(request.email);
    if (!creds) return std::nullopt;
    if (!hasher_.verify(request.password, creds->second)) return std::nullopt;
    auto user = repo_.findById(creds->first);
    if (!user) return std::nullopt;
    return UserProfileDto(*user);
}
