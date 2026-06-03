#include "application/chatSessions/services/CreateChatSession.h"
#include "ChatSessionId.h"
#include "ChatSessionDescription.h"
#include "UuidGenerator.h"
#include <chrono>

CreateChatSession::CreateChatSession(IChatSessionRepository& repo) : repo_(repo) {}

ChatSessionDto CreateChatSession::execute(const CreateChatSessionCommand& request) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    ChatSession session(
        ChatSessionId(UuidGenerator::generate()),
        request.userId,
        request.personaId,
        ChatSessionDescription(request.description, std::nullopt),
        today,
        today
    );
    return ChatSessionDto(repo_.create(session));
}
