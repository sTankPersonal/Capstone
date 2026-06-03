#pragma once
#include "IUseCase.h"
#include "GetChatHistoryQuery.h"
#include "IChatMessageRepository.h"
#include "ChatMessageDto.h"
#include <vector>

class GetChatHistory : public IUseCase<GetChatHistoryQuery, std::vector<ChatMessageDto>> {
    IChatMessageRepository& repo_;
public:
    explicit GetChatHistory(IChatMessageRepository& repo);
    std::vector<ChatMessageDto> execute(const GetChatHistoryQuery& request) override;
};
