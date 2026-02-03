#pragma once

#include "IValueObject.h"
#include <string>
#include <optional>

class ChatMessageContent : public IValueObject {
    std::optional<std::string> content_;

public:
    ChatMessageContent() = default;
    explicit ChatMessageContent(const std::optional<std::string>& content);

    const std::optional<std::string>& getContent() const noexcept;
};
