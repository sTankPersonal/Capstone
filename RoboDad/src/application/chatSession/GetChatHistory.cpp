#include "application/chatSession/GetChatHistory.h"

GetChatHistory::GetChatHistory(IChatMessageRepository& repo) : repo_(repo) {}

std::vector<ChatMessage> GetChatHistory::execute(const ChatSessionId& sessionId) {
    return repo_.findByChatSessionId(sessionId);
}
