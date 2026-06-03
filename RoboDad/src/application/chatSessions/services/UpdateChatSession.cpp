#include "application/chatSessions/services/UpdateChatSession.h"
#include "ChatSessionDescription.h"
#include <chrono>

UpdateChatSession::UpdateChatSession(IChatSessionRepository& repo) : repo_(repo) {}

bool UpdateChatSession::execute(const UpdateChatSessionCommand& request) {
    auto session = repo_.findById(request.sessionId);
    if (!session) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    ChatSession updated(
        request.sessionId,
        session->getUserId(),
        session->getPersonaId(),
        ChatSessionDescription(request.description, request.additionalInfo),
        session->getCreatedAt(),
        today
    );
    return repo_.update(updated);
}
