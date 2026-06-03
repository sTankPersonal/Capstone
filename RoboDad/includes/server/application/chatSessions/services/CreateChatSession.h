#pragma once
#include "IUseCase.h"
#include "CreateChatSessionCommand.h"
#include "IChatSessionRepository.h"
#include "ChatSessionDto.h"

class CreateChatSession : public IUseCase<CreateChatSessionCommand, ChatSessionDto> {
    IChatSessionRepository& repo_;
public:
    explicit CreateChatSession(IChatSessionRepository& repo);
    ChatSessionDto execute(const CreateChatSessionCommand& request) override;
};
