#pragma once
#include "IUseCase.h"
#include "GetChatSessionQuery.h"
#include "IChatSessionRepository.h"
#include "ILlmPersonaRepository.h"
#include "ChatSessionViewDto.h"
#include <optional>

class GetChatSessionView : public IUseCase<GetChatSessionQuery, std::optional<ChatSessionViewDto>> {
    IChatSessionRepository& sessionRepo_;
    ILlmPersonaRepository&  personaRepo_;
public:
    GetChatSessionView(IChatSessionRepository& sessionRepo, ILlmPersonaRepository& personaRepo);
    std::optional<ChatSessionViewDto> execute(const GetChatSessionQuery& request) override;
};
