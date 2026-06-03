#pragma once
#include "ChatSessionId.h"

struct GetChatHistoryQuery {
    ChatSessionId sessionId;

    explicit GetChatHistoryQuery(ChatSessionId sessionId)
        : sessionId(std::move(sessionId)) {}
};
