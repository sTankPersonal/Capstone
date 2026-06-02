#include "application/chatSession/DeleteChatSession.h"

DeleteChatSession::DeleteChatSession(IChatSessionRepository& repo) : repo_(repo) {}

bool DeleteChatSession::execute(const ChatSessionId& sessionId) {
    return repo_.remove(sessionId);
}
