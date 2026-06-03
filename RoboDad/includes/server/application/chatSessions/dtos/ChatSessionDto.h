#pragma once
#include "IDto.h"
#include "ChatSession.h"
#include <string>
#include <optional>

class ChatSessionDto : public IDto {
    std::string id_;
    std::string userId_;
    std::string personaId_;
    std::string description_;
    std::optional<std::string> additionalInfo_;
    std::string createdAt_;
    std::string updatedAt_;

public:
    explicit ChatSessionDto(const ChatSession& session);
    explicit operator crow::json::wvalue() const override;

    std::string getUserId() const;
};
