#pragma once
#include "LlmPersonaId.h"

struct DeletePersonaCommand {
    LlmPersonaId id;

    explicit DeletePersonaCommand(LlmPersonaId id)
        : id(std::move(id)) {}
};
