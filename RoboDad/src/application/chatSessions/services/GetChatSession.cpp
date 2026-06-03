#include "application/chatSessions/services/GetChatSession.h"

GetChatSession::GetChatSession(IChatSessionRepository& repo) : repo_(repo) {}

std::optional<ChatSessionDto> GetChatSession::execute(const GetChatSessionQuery& request) {
    auto session = repo_.findById(request.sessionId);
    if (!session) return std::nullopt;
    return ChatSessionDto(*session);
}
