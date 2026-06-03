#include "application/chatSessions/services/ListChatSessions.h"

ListChatSessions::ListChatSessions(IChatSessionRepository& repo) : repo_(repo) {}

std::vector<ChatSessionDto> ListChatSessions::execute(const ListChatSessionsQuery& request) {
    auto sessions = repo_.findByUserId(request.userId);
    std::vector<ChatSessionDto> dtos;
    dtos.reserve(sessions.size());
    for (const auto& s : sessions) dtos.emplace_back(s);
    return dtos;
}
