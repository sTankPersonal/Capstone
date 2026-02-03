#include "application/chatSession/ListChatSessions.h"

ListChatSessions::ListChatSessions(IChatSessionRepository& repo) : repo_(repo) {}

std::vector<ChatSession> ListChatSessions::execute(const UserId& userId) {
    return repo_.findByUserId(userId);
}
