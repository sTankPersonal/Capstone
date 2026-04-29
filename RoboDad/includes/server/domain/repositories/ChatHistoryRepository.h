#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "llm/ChatHistory.hpp"

class ChatHistoryRepository {
public:
    virtual ~ChatHistoryRepository() = default;
    virtual ChatHistory create(uint32_t promptId, const ChatHistory& history) = 0;
    virtual std::optional<ChatHistory> findById(uint32_t id) = 0;
    virtual std::vector<ChatHistory> findByPromptId(uint32_t promptId) = 0;
    virtual bool update(const ChatHistory& history) = 0;
    virtual bool remove(uint32_t id) = 0;
};
