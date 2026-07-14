#include "application/chatSessions/services/ListChatSessionsView.h"

ListChatSessionsView::ListChatSessionsView(IChatSessionRepository& sessionRepo, ILlmPersonaRepository& personaRepo)
    : sessionRepo_(sessionRepo), personaRepo_(personaRepo) {}

std::vector<ChatSessionViewDto> ListChatSessionsView::execute(const ListChatSessionsQuery& request) {
    auto sessions = sessionRepo_.findByUserId(request.userId);
    std::vector<ChatSessionViewDto> views;
    views.reserve(sessions.size());
    for (const auto& session : sessions) {
        auto persona = personaRepo_.findById(session.getPersonaId());
        views.emplace_back(session, *persona);
    }
    return views;
}
