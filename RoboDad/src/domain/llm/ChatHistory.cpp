#include "domain/llm/ChatHistory.h"

ChatHistory::ChatHistory(uint32_t id, uint64_t timestamp, const std::string& userMessage, const std::string& llmResponse)
    : Id(id), Timestamp(timestamp), UserMessage(userMessage), LLMResponse(llmResponse) {}

uint32_t ChatHistory::getId() const { return Id; }
uint64_t ChatHistory::getTimestamp() const { return Timestamp; }
std::string ChatHistory::getUserMessage() const { return UserMessage; }
std::string ChatHistory::getLLMResponse() const { return LLMResponse; }

void ChatHistory::setId(uint32_t newId) { Id = newId; }
void ChatHistory::setTimestamp(uint64_t newTimestamp) { Timestamp = newTimestamp; }
void ChatHistory::setUserMessage(const std::string& newUserMessage) { UserMessage = newUserMessage; }
void ChatHistory::setLLMResponse(const std::string& newLLMResponse) { LLMResponse = newLLMResponse; }
