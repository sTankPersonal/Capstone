#pragma once
#include "ChatSessionId.h"
#include <optional>
#include <string>

struct UpdateChatSessionCommand {
    ChatSessionId sessionId;
    std::string description;
    std::optional<std::string> additionalInfo;

    explicit UpdateChatSessionCommand(ChatSessionId sessionId, std::string description,
                             std::optional<std::string> additionalInfo = std::nullopt)
        : sessionId(std::move(sessionId)), description(std::move(description)),
          additionalInfo(std::move(additionalInfo)) {}
};
