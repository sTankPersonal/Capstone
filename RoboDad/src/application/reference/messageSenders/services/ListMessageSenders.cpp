#include "application/references/messageSenders/services/ListMessageSenders.h"

ListMessageSenders::ListMessageSenders(IMessageSenderRepository& repo) : repo_(repo) {}

std::vector<MessageSenderDto> ListMessageSenders::execute(const ListMessageSendersQuery&) {
    auto items = repo_.findAll();
    std::vector<MessageSenderDto> dtos;
    dtos.reserve(items.size());
    for (const auto& s : items) dtos.emplace_back(s);
    return dtos;
}
