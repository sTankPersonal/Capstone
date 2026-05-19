#pragma once
#include "domain/IEntity.h"
#include <cstdint>
#include <string>

class ChatHistory : public IEntity<uint32_t> {
    uint32_t Id;
    uint64_t Timestamp;
    std::string UserMessage;
    std::string LLMResponse;

public:
    ChatHistory(uint32_t id, uint64_t timestamp, const std::string& userMessage, const std::string& llmResponse);

    uint32_t getId() const override;
    uint64_t getTimestamp() const;
    std::string getUserMessage() const;
    std::string getLLMResponse() const;

    void setId(uint32_t newId);
    void setTimestamp(uint64_t newTimestamp);
    void setUserMessage(const std::string& newUserMessage);
    void setLLMResponse(const std::string& newLLMResponse);
};
