#pragma once
#include "IUseCase.h"
#include "ListMessageSendersQuery.h"
#include "IMessageSenderRepository.h"
#include "MessageSenderDto.h"
#include <vector>

class ListMessageSenders : public IUseCase<ListMessageSendersQuery, std::vector<MessageSenderDto>> {
    IMessageSenderRepository& repo_;
public:
    explicit ListMessageSenders(IMessageSenderRepository& repo);
    std::vector<MessageSenderDto> execute(const ListMessageSendersQuery& request) override;
};
