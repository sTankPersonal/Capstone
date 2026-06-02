#pragma once
#include "ILlmPersonaRepository.h"
#include "LlmPersona.h"
#include "LlmPersonaId.h"
#include <optional>

class GetPersona {
    ILlmPersonaRepository& repo_;
public:
    explicit GetPersona(ILlmPersonaRepository& repo);
    std::optional<LlmPersona> execute(const LlmPersonaId& id);
};
