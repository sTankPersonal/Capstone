#include "application/auth/LoginUser.h"

LoginUser::LoginUser(IUserRepository& repo, IPasswordHasher& hasher)
    : repo_(repo), hasher_(hasher) {}

std::optional<User> LoginUser::execute(const std::string& email, const std::string& password) {
    auto creds = repo_.lookupCredentials(email);
    if (!creds) return std::nullopt;
    if (!hasher_.verify(password, creds->second)) return std::nullopt;
    return repo_.findById(creds->first);
}
