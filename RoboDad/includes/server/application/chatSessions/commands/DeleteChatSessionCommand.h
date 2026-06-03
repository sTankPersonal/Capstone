#pragma once
#include "ChatSessionId.h"

struct DeleteChatSessionCommand {
    ChatSessionId sessionId;

    explicit DeleteChatSessionCommand(ChatSessionId sessionId)
        : sessionId(std::move(sessionId)) {}
};
