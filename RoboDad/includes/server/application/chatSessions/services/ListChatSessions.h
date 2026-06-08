#pragma once
#include "IUseCase.h"
#include "ListChatSessionsQuery.h"
#include "IChatSessionRepository.h"
#include "ChatSessionDto.h"
#include <vector>

class ListChatSessions : public IUseCase<ListChatSessionsQuery, std::vector<ChatSessionDto>> {
    IChatSessionRepository& repo_;
public:
    explicit ListChatSessions(IChatSessionRepository& repo);
    std::vector<ChatSessionDto> execute(const ListChatSessionsQuery& request) override;
};
