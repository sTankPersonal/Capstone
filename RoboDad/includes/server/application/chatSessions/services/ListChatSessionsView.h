#pragma once
#include "IUseCase.h"
#include "ListChatSessionsQuery.h"
#include "IChatSessionRepository.h"
#include "ILlmPersonaRepository.h"
#include "ChatSessionViewDto.h"
#include <vector>

class ListChatSessionsView : public IUseCase<ListChatSessionsQuery, std::vector<ChatSessionViewDto>> {
    IChatSessionRepository& sessionRepo_;
    ILlmPersonaRepository&  personaRepo_;
public:
    ListChatSessionsView(IChatSessionRepository& sessionRepo, ILlmPersonaRepository& personaRepo);
    std::vector<ChatSessionViewDto> execute(const ListChatSessionsQuery& request) override;
};
