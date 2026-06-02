#include "ChatSessionId.h"

ChatSessionId::ChatSessionId(const std::string& id) : id_(id) {}

const std::string& ChatSessionId::getId() const noexcept { return id_; }
