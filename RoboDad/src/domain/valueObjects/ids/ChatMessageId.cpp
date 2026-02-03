#include "ChatMessageId.h"

ChatMessageId::ChatMessageId(const std::string& id) : id_(id) {}

const std::string& ChatMessageId::getId() const noexcept { return id_; }
