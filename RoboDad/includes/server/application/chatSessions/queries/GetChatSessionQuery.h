#pragma once
#include "ChatSessionId.h"

struct GetChatSessionQuery {
    ChatSessionId sessionId;

    explicit GetChatSessionQuery(ChatSessionId sessionId)
        : sessionId(std::move(sessionId)) {}
};
