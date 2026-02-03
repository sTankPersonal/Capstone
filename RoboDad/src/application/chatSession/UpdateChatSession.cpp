#include "application/chatSession/UpdateChatSession.h"
#include "ChatSessionDescription.h"
#include <chrono>

UpdateChatSession::UpdateChatSession(IChatSessionRepository& repo) : repo_(repo) {}

bool UpdateChatSession::execute(const ChatSessionId&            sessionId,
                                 const std::string&              description,
                                 const std::optional<std::string>& additionalInfo) {
    auto session = repo_.findById(sessionId);
    if (!session) return false;
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    ChatSession updated(
        sessionId,
        session->getUserId(),
        session->getPersonaId(),
        ChatSessionDescription(description, additionalInfo),
        session->getCreatedAt(),
        today
    );
    return repo_.update(updated);
}
