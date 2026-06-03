#pragma once
#include "ChatSessionId.h"
#include <string>

struct SendChatMessageCommand {
    ChatSessionId sessionId;
    std::string userMessage;

    explicit SendChatMessageCommand(ChatSessionId sessionId, std::string userMessage)
        : sessionId(std::move(sessionId)), userMessage(std::move(userMessage)) {}
};
