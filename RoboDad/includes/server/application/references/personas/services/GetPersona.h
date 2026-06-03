#pragma once
#include "IUseCase.h"
#include "GetPersonaQuery.h"
#include "ILlmPersonaRepository.h"
#include "PersonaDto.h"
#include <optional>

class GetPersona : public IUseCase<GetPersonaQuery, std::optional<PersonaDto>> {
    ILlmPersonaRepository& repo_;
public:
    explicit GetPersona(ILlmPersonaRepository& repo);
    std::optional<PersonaDto> execute(const GetPersonaQuery& request) override;
};
