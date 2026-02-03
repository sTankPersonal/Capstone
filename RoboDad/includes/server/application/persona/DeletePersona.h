#pragma once
#include "ILlmPersonaRepository.h"
#include "LlmPersonaId.h"

class DeletePersona {
    ILlmPersonaRepository& repo_;
public:
    explicit DeletePersona(ILlmPersonaRepository& repo);
    bool execute(const LlmPersonaId& id);
};
