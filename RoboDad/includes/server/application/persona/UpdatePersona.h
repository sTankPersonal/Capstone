#pragma once
#include "ILlmPersonaRepository.h"
#include "LlmPersonaId.h"
#include <string>

class UpdatePersona {
    ILlmPersonaRepository& repo_;
public:
    explicit UpdatePersona(ILlmPersonaRepository& repo);
    bool execute(const LlmPersonaId& id,
                 const std::string&  name,
                 const std::string&  description,
                 const std::string&  systemPrompt);
};
