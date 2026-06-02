#pragma once

#include "IEntity.h"
#include "ChatMessageId.h"
#include "ChatSessionId.h"
#include "MessageSenderId.h"
#include "ChatMessageContent.h"
#include <chrono>

class ChatMessage : public IEntity<ChatMessageId, std::chrono::year_month_day>
{
    ChatMessageId chatMessageId_;
    ChatSessionId chatSessionId_;
    MessageSenderId messageSenderId_;
    ChatMessageContent content_;
    std::chrono::year_month_day createdAt_;

public:
    explicit ChatMessage(const ChatMessageId& chatMessageId, const ChatSessionId& chatSessionId, const MessageSenderId& messageSenderId, const ChatMessageContent& content, const std::chrono::year_month_day& timestamp);

    const ChatMessageId& getId() const noexcept override;
    const ChatSessionId& getChatSessionId() const noexcept;
    const MessageSenderId& getMessageSenderId() const noexcept;
    const ChatMessageContent& getContent() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setContent(const ChatMessageContent& content);
};
