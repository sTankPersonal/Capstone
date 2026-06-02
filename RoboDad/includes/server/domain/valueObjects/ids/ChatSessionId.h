#pragma once

#include "IIdValueObject.h"
#include <string>

class ChatSessionId : public IIdValueObject<std::string> {
    std::string id_;

public:
    ChatSessionId() = default;
    explicit ChatSessionId(const std::string& id);

    const std::string& getId() const noexcept override;
};
