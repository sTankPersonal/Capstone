#pragma once
#include "domain/repositories/ChatHistoryRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresChatHistoryRepository : public ChatHistoryRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresChatHistoryRepository(DatabaseConnection& db);

    ChatHistory create(uint32_t promptId, const ChatHistory& history) override;
    std::optional<ChatHistory> findById(uint32_t id) override;
    std::vector<ChatHistory> findByPromptId(uint32_t promptId) override;
    bool update(const ChatHistory& history) override;
    bool remove(uint32_t id) override;
};
