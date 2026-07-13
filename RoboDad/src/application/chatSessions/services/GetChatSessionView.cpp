#include "application/chatSessions/services/GetChatSessionView.h"

GetChatSessionView::GetChatSessionView(IChatSessionRepository& sessionRepo, ILlmPersonaRepository& personaRepo)
    : sessionRepo_(sessionRepo), personaRepo_(personaRepo) {}

std::optional<ChatSessionViewDto> GetChatSessionView::execute(const GetChatSessionQuery& request) {
    auto session = sessionRepo_.findById(request.sessionId);
    if (!session) return std::nullopt;

    auto persona = personaRepo_.findById(session->getPersonaId());
    return ChatSessionViewDto(*session, *persona);
}
