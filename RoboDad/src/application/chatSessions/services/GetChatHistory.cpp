#include "application/chatSessions/services/GetChatHistory.h"

GetChatHistory::GetChatHistory(IChatMessageRepository& repo) : repo_(repo) {}

std::vector<ChatMessageDto> GetChatHistory::execute(const GetChatHistoryQuery& request) {
    auto messages = repo_.findByChatSessionId(request.sessionId);
    std::vector<ChatMessageDto> dtos;
    dtos.reserve(messages.size());
    for (const auto& m : messages) dtos.emplace_back(m);
    return dtos;
}
