#pragma once
#include "IUseCase.h"
#include "DeleteChatSessionCommand.h"
#include "IChatSessionRepository.h"

class DeleteChatSession : public IUseCase<DeleteChatSessionCommand, bool> {
    IChatSessionRepository& repo_;
public:
    explicit DeleteChatSession(IChatSessionRepository& repo);
    bool execute(const DeleteChatSessionCommand& request) override;
};
