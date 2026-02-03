#pragma once

#include "IIdValueObject.h"
#include <string>

class MessageSenderId : public IIdValueObject<std::string> {
    std::string id_;

public:
    MessageSenderId() = default;
    explicit MessageSenderId(const std::string& id);

    const std::string& getId() const noexcept override;
};
