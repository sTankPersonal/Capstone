#include "application/auth/LoginUser.h"

std::optional<User> LoginUser::execute(uint32_t userId) {
    return repo_.findById(userId);
}
