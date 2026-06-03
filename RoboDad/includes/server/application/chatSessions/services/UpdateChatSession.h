#pragma once
#include "IUseCase.h"
#include "UpdateChatSessionCommand.h"
#include "IChatSessionRepository.h"

class UpdateChatSession : public IUseCase<UpdateChatSessionCommand, bool> {
    IChatSessionRepository& repo_;
public:
    explicit UpdateChatSession(IChatSessionRepository& repo);
    bool execute(const UpdateChatSessionCommand& request) override;
};
