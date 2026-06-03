#pragma once
#include "IRepository.h"
#include "MessageSender.h"
#include "MessageSenderId.h"
#include <optional>
#include <string>

class IMessageSenderRepository : public IRepository<MessageSender, MessageSenderId> {
public:
    virtual std::optional<MessageSender> findByName(const std::string& name) = 0;
};
