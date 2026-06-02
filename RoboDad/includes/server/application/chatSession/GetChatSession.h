#pragma once
#include "IChatSessionRepository.h"
#include "ChatSession.h"
#include "ChatSessionId.h"
#include <optional>

class GetChatSession {
    IChatSessionRepository& repo_;
public:
    explicit GetChatSession(IChatSessionRepository& repo);
    std::optional<ChatSession> execute(const ChatSessionId& sessionId);
};
