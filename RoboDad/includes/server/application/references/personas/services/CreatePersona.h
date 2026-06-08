#pragma once
#include "IUseCase.h"
#include "CreatePersonaCommand.h"
#include "ILlmPersonaRepository.h"
#include "PersonaDto.h"

class CreatePersona : public IUseCase<CreatePersonaCommand, PersonaDto> {
    ILlmPersonaRepository& repo_;
public:
    explicit CreatePersona(ILlmPersonaRepository& repo);
    PersonaDto execute(const CreatePersonaCommand& request) override;
};
