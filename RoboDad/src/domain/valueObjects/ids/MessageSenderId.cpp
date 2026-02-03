#include "MessageSenderId.h"

MessageSenderId::MessageSenderId(const std::string& id) : id_(id) {}

const std::string& MessageSenderId::getId() const noexcept { return id_; }
