#pragma once
#include "IUseCase.h"
#include "UpdatePersonaCommand.h"
#include "ILlmPersonaRepository.h"

class UpdatePersona : public IUseCase<UpdatePersonaCommand, bool> {
    ILlmPersonaRepository& repo_;
public:
    explicit UpdatePersona(ILlmPersonaRepository& repo);
    bool execute(const UpdatePersonaCommand& request) override;
};
