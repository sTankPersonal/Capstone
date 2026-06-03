#pragma once
#include "IUseCase.h"
#include "ListPersonasQuery.h"
#include "ILlmPersonaRepository.h"
#include "PersonaDto.h"
#include <vector>

class ListPersonas : public IUseCase<ListPersonasQuery, std::vector<PersonaDto>> {
    ILlmPersonaRepository& repo_;
public:
    explicit ListPersonas(ILlmPersonaRepository& repo);
    std::vector<PersonaDto> execute(const ListPersonasQuery& request) override;
};
