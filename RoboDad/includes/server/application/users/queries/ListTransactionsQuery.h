#pragma once
#include "UserId.h"

struct ListTransactionsQuery {
    UserId userId;

    explicit ListTransactionsQuery(UserId userId)
        : userId(std::move(userId)) {}
};
