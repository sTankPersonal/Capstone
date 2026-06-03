#pragma once
#include "IUseCase.h"
#include "GetChatSessionQuery.h"
#include "IChatSessionRepository.h"
#include "ChatSessionDto.h"
#include <optional>

class GetChatSession : public IUseCase<GetChatSessionQuery, std::optional<ChatSessionDto>> {
    IChatSessionRepository& repo_;
public:
    explicit GetChatSession(IChatSessionRepository& repo);
    std::optional<ChatSessionDto> execute(const GetChatSessionQuery& request) override;
};
