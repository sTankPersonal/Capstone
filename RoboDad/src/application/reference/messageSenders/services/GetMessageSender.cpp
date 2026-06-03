#include "application/references/messageSenders/services/GetMessageSender.h"

GetMessageSender::GetMessageSender(IMessageSenderRepository& repo) : repo_(repo) {}

std::optional<MessageSenderDto> GetMessageSender::execute(const GetMessageSenderQuery& request) {
    auto sender = repo_.findById(request.id);
    if (!sender) return std::nullopt;
    return MessageSenderDto(*sender);
}
