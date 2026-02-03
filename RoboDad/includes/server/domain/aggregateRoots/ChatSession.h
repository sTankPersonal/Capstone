#pragma once

#include "IAggregateRoot.h"
#include "ChatSessionId.h"
#include "UserId.h"
#include "LlmPersonaId.h"
#include "ChatSessionDescription.h"
#include <chrono>
#include <string>

class ChatSession : public IAggregateRoot<ChatSessionId, std::chrono::year_month_day, std::chrono::year_month_day>
{
    ChatSessionId id_;
    UserId userId_;
    LlmPersonaId personaId_;
    ChatSessionDescription sessionDescription_;
    std::chrono::year_month_day createdAt_;
    std::chrono::year_month_day updatedAt_;

public:
    explicit ChatSession(
        const ChatSessionId& id,
        const UserId& userId,
        const LlmPersonaId& personaId,
        const ChatSessionDescription& sessionDescription,
        const std::chrono::year_month_day& createdAt,
        const std::chrono::year_month_day& updatedAt
    );

    const ChatSessionId& getId() const noexcept override;
    const UserId& getUserId() const noexcept;
    const LlmPersonaId& getPersonaId() const noexcept;
    const ChatSessionDescription& getSessionDescription() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
    const std::chrono::year_month_day& getUpdatedAt() const noexcept override;

protected:
    void setPersonaId(const LlmPersonaId& personaId);
    void setSessionDescription(const ChatSessionDescription& sessionDescription);
    void setUpdatedAt(const std::chrono::year_month_day& updatedAt) override;
};
