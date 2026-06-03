#pragma once
#include "IDto.h"
#include "ChatMessage.h"
#include <string>
#include <optional>
#include "crow.h"

class ChatMessageDto : public IDto {
    std::string id_;
    std::string sessionId_;
    std::string messageSenderId_;
    std::optional<std::string> content_;
    std::string createdAt_;

public:
    explicit ChatMessageDto(const ChatMessage& message);
    explicit operator crow::json::wvalue() const override;
};
