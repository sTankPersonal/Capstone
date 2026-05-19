#include "application/auth/LoginUser.h"

std::optional<User> LoginUser::execute(const std::string& email, const std::string& password) {
    auto creds = repo_.lookupCredentials(email);
    if (!creds) return std::nullopt;
    if (!hasher_.verify(password, creds->second)) return std::nullopt;
    return repo_.findById(creds->first);
}
