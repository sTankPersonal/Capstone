#pragma once
#include "UserId.h"

struct ListChatSessionsQuery {
    UserId userId;

    explicit ListChatSessionsQuery(UserId userId)
        : userId(std::move(userId)) {}
};
