#pragma once
#include "UserId.h"
#include "PlaidItemId.h"
#include <string>

struct ImportPlaidTransactionsCommand {
    UserId userId;
    PlaidItemId plaidItemId;
    std::string accessToken;
    std::string cursor;

    explicit ImportPlaidTransactionsCommand(UserId userId, PlaidItemId plaidItemId,
                                            std::string accessToken, std::string cursor = "")
        : userId(std::move(userId)), plaidItemId(std::move(plaidItemId))
        , accessToken(std::move(accessToken)), cursor(std::move(cursor)) {}
};
