#pragma once

#include "IChatSessionRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <vector>

class PostgresChatSessionRepository : public IChatSessionRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresChatSessionRepository(DatabaseConnection& db);

    ChatSession create(const ChatSession& session) override;
    std::optional<ChatSession> findById(ChatSessionId id) override;
    std::vector<ChatSession> findAll() override;
    bool update(const ChatSession& session) override;
    bool remove(ChatSessionId id) override;

    // Returns all sessions belonging to the given user.
    std::vector<ChatSession> findByUserId(const UserId& userId);
};
