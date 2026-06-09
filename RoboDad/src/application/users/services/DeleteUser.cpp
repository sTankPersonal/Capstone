#include "application/users/services/DeleteUser.h"

DeleteUser::DeleteUser(IUserRepository& repo) : repo_(repo) {}

bool DeleteUser::execute(const DeleteUserCommand& request) {
    return repo_.remove(request.userId);
}
