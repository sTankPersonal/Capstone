#pragma once
#include "LlmPersonaId.h"
#include <string>

struct UpdatePersonaCommand {
    LlmPersonaId id;
    std::string name;
    std::string description;
    std::string systemPrompt;

    UpdatePersonaCommand(LlmPersonaId id, std::string name, std::string description, std::string systemPrompt)
        : id(std::move(id)), name(std::move(name)), description(std::move(description)),
          systemPrompt(std::move(systemPrompt)) {}
};
