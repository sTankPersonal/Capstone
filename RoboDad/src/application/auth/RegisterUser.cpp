#include "application/auth/RegisterUser.h"

User RegisterUser::execute(const PersonalInfo& info) {
    User newUser(0, info, {});
    return repo_.create(newUser);
}
