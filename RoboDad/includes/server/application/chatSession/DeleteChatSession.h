#pragma once
#include "IChatSessionRepository.h"
#include "ChatSessionId.h"

class DeleteChatSession {
    IChatSessionRepository& repo_;
public:
    explicit DeleteChatSession(IChatSessionRepository& repo);
    bool execute(const ChatSessionId& sessionId);
};
