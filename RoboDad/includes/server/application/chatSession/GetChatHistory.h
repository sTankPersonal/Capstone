#pragma once
#include "IChatMessageRepository.h"
#include "ChatMessage.h"
#include "ChatSessionId.h"
#include <vector>

class GetChatHistory {
    IChatMessageRepository& repo_;
public:
    explicit GetChatHistory(IChatMessageRepository& repo);
    std::vector<ChatMessage> execute(const ChatSessionId& sessionId);
};
