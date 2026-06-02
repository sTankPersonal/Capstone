#pragma once
#include "IChatSessionRepository.h"
#include "ChatSessionId.h"
#include <optional>
#include <string>

class UpdateChatSession {
    IChatSessionRepository& repo_;
public:
    explicit UpdateChatSession(IChatSessionRepository& repo);
    bool execute(const ChatSessionId& sessionId,
                 const std::string& description,
                 const std::optional<std::string>& additionalInfo = std::nullopt);
};
