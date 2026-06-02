#pragma once
#include "ILlmPersonaRepository.h"
#include "LlmPersona.h"
#include <vector>

class ListPersonas {
    ILlmPersonaRepository& repo_;
public:
    explicit ListPersonas(ILlmPersonaRepository& repo);
    std::vector<LlmPersona> execute();
};
