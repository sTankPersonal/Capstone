#pragma once
#include "IRepository.h"
#include "ChatMessage.h"
#include "ChatMessageId.h"
#include "ChatSessionId.h"
#include <vector>

class IChatMessageRepository : public IRepository<ChatMessage, ChatMessageId> {
public:
    virtual std::vector<ChatMessage> findByChatSessionId(const ChatSessionId& sessionId) = 0;
    virtual std::vector<ChatMessage> findByChatSessionId(const ChatSessionId& sessionId, int limit) = 0;
};
