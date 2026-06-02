#pragma once

#include "IChatMessageRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <vector>

class PostgresChatMessageRepository : public IChatMessageRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresChatMessageRepository(DatabaseConnection& db);

    ChatMessage create(const ChatMessage& message) override;
    std::optional<ChatMessage> findById(ChatMessageId id) override;
    std::vector<ChatMessage> findAll() override;
    bool update(const ChatMessage& message) override;
    bool remove(ChatMessageId id) override;

    // Returns all messages belonging to the given chat session, in insertion order.
    std::vector<ChatMessage> findByChatSessionId(const ChatSessionId& sessionId);
    std::vector<ChatMessage> findByChatSessionId(const ChatSessionId& sessionId, int limit);
};
