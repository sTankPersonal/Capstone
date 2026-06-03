#pragma once
#include "LlmPersonaId.h"

struct GetPersonaQuery {
    LlmPersonaId id;

    explicit GetPersonaQuery(LlmPersonaId id)
        : id(std::move(id)) {}
};
