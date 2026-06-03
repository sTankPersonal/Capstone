#pragma once
#include "UserId.h"
#include "LlmPersonaId.h"
#include <string>

struct CreateChatSessionCommand {
    UserId userId;
    LlmPersonaId personaId;
    std::string description;

    explicit CreateChatSessionCommand(UserId userId, LlmPersonaId personaId, std::string description)
        : userId(std::move(userId)), personaId(std::move(personaId)), description(std::move(description)) {}
};
