#pragma once
#include "MessageSenderId.h"

struct GetMessageSenderQuery {
    MessageSenderId id;

    explicit GetMessageSenderQuery(MessageSenderId id)
        : id(std::move(id)) {}
};
