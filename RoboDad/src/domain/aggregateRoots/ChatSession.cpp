#include "ChatSession.h"

ChatSession::ChatSession(
    const ChatSessionId& id,
    const UserId& userId,
    const LlmPersonaId& personaId,
    const ChatSessionDescription& sessionDescription,
    const std::chrono::year_month_day& createdAt,
    const std::chrono::year_month_day& updatedAt
) : id_(id), userId_(userId), personaId_(personaId), sessionDescription_(sessionDescription), createdAt_(createdAt), updatedAt_(updatedAt) {}

const ChatSessionId& ChatSession::getId() const noexcept { return id_; }
const UserId& ChatSession::getUserId() const noexcept { return userId_; }
const LlmPersonaId& ChatSession::getPersonaId() const noexcept { return personaId_; }
const ChatSessionDescription& ChatSession::getSessionDescription() const noexcept { return sessionDescription_; }
const std::chrono::year_month_day& ChatSession::getCreatedAt() const noexcept { return createdAt_; }
const std::chrono::year_month_day& ChatSession::getUpdatedAt() const noexcept { return updatedAt_; }

void ChatSession::setPersonaId(const LlmPersonaId& personaId) { personaId_ = personaId; }
void ChatSession::setSessionDescription(const ChatSessionDescription& sessionDescription) { sessionDescription_ = sessionDescription; }
void ChatSession::setUpdatedAt(const std::chrono::year_month_day& updatedAt) { updatedAt_ = updatedAt; }
