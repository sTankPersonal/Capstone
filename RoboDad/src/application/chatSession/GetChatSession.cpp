#include "application/chatSession/GetChatSession.h"

GetChatSession::GetChatSession(IChatSessionRepository& repo) : repo_(repo) {}

std::optional<ChatSession> GetChatSession::execute(const ChatSessionId& sessionId) {
    return repo_.findById(sessionId);
}
