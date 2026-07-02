#include "application/users/services/CreatePlaidLinkToken.h"

CreatePlaidLinkToken::CreatePlaidLinkToken(IPlaidClient& plaidClient)
    : plaidClient_(plaidClient) {}

std::string CreatePlaidLinkToken::execute(const CreatePlaidLinkTokenCommand& request) {
    return plaidClient_.createLinkToken(request.userId.getId());
}
