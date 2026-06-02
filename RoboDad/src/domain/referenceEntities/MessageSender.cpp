#include "MessageSender.h"

MessageSender::MessageSender(
    const MessageSenderId& id,
    const std::string& name,
    const std::chrono::year_month_day& createdAt
) : id_(id), name_(name), createdAt_(createdAt) {}

const MessageSenderId& MessageSender::getId() const noexcept { return id_; }
const std::string& MessageSender::getName() const noexcept { return name_; }
const std::chrono::year_month_day& MessageSender::getCreatedAt() const noexcept { return createdAt_; }

void MessageSender::setName(const std::string& name) { name_ = name; }
