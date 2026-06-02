#include "application/chatSession/CreateChatSession.h"
#include "ChatSessionId.h"
#include "ChatSessionDescription.h"
#include "UuidGenerator.h"
#include <chrono>

CreateChatSession::CreateChatSession(IChatSessionRepository& repo) : repo_(repo) {}

ChatSession CreateChatSession::execute(const UserId&      userId,
                                       const LlmPersonaId& personaId,
                                       const std::string& description) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    ChatSession session(
        ChatSessionId(UuidGenerator::generate()),
        userId,
        personaId,
        ChatSessionDescription(description, std::nullopt),
        today,
        today
    );
    return repo_.create(session);
}
