#include "application/chatSessions/dtos/ChatMessageDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

ChatMessageDto::ChatMessageDto(const ChatMessage& message)
    : id_(message.getId().getId())
    , sessionId_(message.getChatSessionId().getId())
    , messageSenderId_(message.getMessageSenderId().getId())
    , createdAt_(formatDate(message.getCreatedAt()))
{
    const auto& c = message.getContent().getContent();
    if (c) content_ = *c;
}

ChatMessageDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]              = id_;
    result["sessionId"]       = sessionId_;
    result["messageSenderId"] = messageSenderId_;
    if (content_) result["content"] = *content_;
    result["createdAt"]       = createdAt_;
    return result;
}
