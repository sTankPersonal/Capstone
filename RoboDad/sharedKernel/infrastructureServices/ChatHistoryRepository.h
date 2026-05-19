#pragma once
#include <cstdint>
#include <vector>
#include "IRepository.h"
#include "llm/ChatHistory.hpp"

class ChatHistoryRepository : public IRepository<ChatHistory> {
public:
    virtual ChatHistory create(uint32_t promptId, const ChatHistory& history) = 0;
    virtual std::vector<ChatHistory> findByPromptId(uint32_t promptId) = 0;
};
