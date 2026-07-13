#pragma once
#include "IDto.h"
#include "ChatSession.h"
#include "LlmPersona.h"
#include <string>
#include <optional>

class ChatSessionViewDto : public IDto {
    std::string id_;
    std::string userId_;
    std::string personaId_;
    std::string personaName_;
    std::string personaDescription_;
    std::string description_;
    std::optional<std::string> additionalInfo_;
    std::string createdAt_;
    std::string updatedAt_;

public:
    ChatSessionViewDto(const ChatSession& session, const LlmPersona& persona);
    explicit operator crow::json::wvalue() const override;

    std::string getUserId() const;
};
