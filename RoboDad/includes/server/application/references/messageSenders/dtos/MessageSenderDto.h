#pragma once
#include "IDto.h"
#include "MessageSender.h"
#include <string>
#include "crow.h"

class MessageSenderDto : public IDto {
    std::string id_;
    std::string name_;
    std::string createdAt_;

public:
    explicit MessageSenderDto(const MessageSender& sender);
    explicit operator crow::json::wvalue() const override;
};
