#include "application/references/messageSenders/dtos/MessageSenderDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

MessageSenderDto::MessageSenderDto(const MessageSender& sender)
    : id_(sender.getId().getId())
    , name_(sender.getName())
    , createdAt_(formatDate(sender.getCreatedAt()))
{}

MessageSenderDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["name"]      = name_;
    result["createdAt"] = createdAt_;
    return result;
}
