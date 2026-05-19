#include "application/auth/RegisterUser.h"

User RegisterUser::execute(const PersonalInfo& info, const std::string& email, const std::string& password) {
    std::string passwordHash = hasher_.hash(password);
    User newUser(0, info, {});
    return repo_.create(newUser, email, passwordHash);
}
