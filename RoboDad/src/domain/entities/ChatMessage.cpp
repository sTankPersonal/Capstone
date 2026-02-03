#include "ChatMessage.h"

ChatMessage::ChatMessage(const ChatMessageId& chatMessageId, const ChatSessionId& chatSessionId, const MessageSenderId& messageSenderId, const ChatMessageContent& content, const std::chrono::year_month_day& timestamp)
    : chatMessageId_(chatMessageId), chatSessionId_(chatSessionId), messageSenderId_(messageSenderId), content_(content), createdAt_(timestamp) {}

const ChatMessageId& ChatMessage::getId() const noexcept { return chatMessageId_; }
const ChatSessionId& ChatMessage::getChatSessionId() const noexcept { return chatSessionId_; }
const MessageSenderId& ChatMessage::getMessageSenderId() const noexcept { return messageSenderId_; }
const ChatMessageContent& ChatMessage::getContent() const noexcept { return content_; }
const std::chrono::year_month_day& ChatMessage::getCreatedAt() const noexcept { return createdAt_; }

void ChatMessage::setContent(const ChatMessageContent& content) { content_ = content; }
