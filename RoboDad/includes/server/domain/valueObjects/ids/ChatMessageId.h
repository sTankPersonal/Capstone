#pragma once

#include "IIdValueObject.h"
#include <string>

class ChatMessageId : public IIdValueObject<std::string> {
    std::string id_;

public:
    ChatMessageId() = default;
    explicit ChatMessageId(const std::string& id);

    const std::string& getId() const noexcept override;
};
