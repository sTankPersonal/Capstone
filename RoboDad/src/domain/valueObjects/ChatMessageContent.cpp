#include "ChatMessageContent.h"

ChatMessageContent::ChatMessageContent(const std::optional<std::string>& content) : content_(content) {}

const std::optional<std::string>& ChatMessageContent::getContent() const noexcept { return content_; }
