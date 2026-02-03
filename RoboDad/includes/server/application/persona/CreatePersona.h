#pragma once
#include "ILlmPersonaRepository.h"
#include "LlmPersona.h"
#include <string>

class CreatePersona {
    ILlmPersonaRepository& repo_;
public:
    explicit CreatePersona(ILlmPersonaRepository& repo);
    LlmPersona execute(const std::string& name,
                       const std::string& description,
                       const std::string& systemPrompt);
};
