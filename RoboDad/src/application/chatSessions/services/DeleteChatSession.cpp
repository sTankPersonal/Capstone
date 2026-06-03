#include "application/chatSessions/services/DeleteChatSession.h"

DeleteChatSession::DeleteChatSession(IChatSessionRepository& repo) : repo_(repo) {}

bool DeleteChatSession::execute(const DeleteChatSessionCommand& request) {
    return repo_.remove(request.sessionId);
}
