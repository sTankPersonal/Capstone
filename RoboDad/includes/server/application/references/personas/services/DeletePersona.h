#pragma once
#include "IUseCase.h"
#include "DeletePersonaCommand.h"
#include "ILlmPersonaRepository.h"

class DeletePersona : public IUseCase<DeletePersonaCommand, bool> {
    ILlmPersonaRepository& repo_;
public:
    explicit DeletePersona(ILlmPersonaRepository& repo);
    bool execute(const DeletePersonaCommand& request) override;
};
