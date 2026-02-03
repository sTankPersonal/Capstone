#pragma once
#include "IChatSessionRepository.h"
#include "ChatSession.h"
#include "UserId.h"
#include "LlmPersonaId.h"
#include <string>

class CreateChatSession {
    IChatSessionRepository& repo_;
public:
    explicit CreateChatSession(IChatSessionRepository& repo);
    ChatSession execute(const UserId& userId, const LlmPersonaId& personaId, const std::string& description);
};
