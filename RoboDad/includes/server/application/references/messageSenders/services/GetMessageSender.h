#pragma once
#include "IUseCase.h"
#include "GetMessageSenderQuery.h"
#include "IMessageSenderRepository.h"
#include "MessageSenderDto.h"
#include <optional>

class GetMessageSender : public IUseCase<GetMessageSenderQuery, std::optional<MessageSenderDto>> {
    IMessageSenderRepository& repo_;
public:
    explicit GetMessageSender(IMessageSenderRepository& repo);
    std::optional<MessageSenderDto> execute(const GetMessageSenderQuery& request) override;
};
