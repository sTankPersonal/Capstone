#pragma once
#include <string>

class ChatHistory {
	uint32_t Id;
	uint64_t Timestamp;

	std::string UserMessage;
	std::string LLMResponse;

public:
	ChatHistory(uint32_t id, uint64_t timestamp, const std::string& userMessage, const std::string& llmResponse)
		: Id(id), Timestamp(timestamp), UserMessage(userMessage), LLMResponse(llmResponse) {
	}

	uint32_t getId() const { return Id; }
	uint64_t getTimestamp() const { return Timestamp; }
	std::string getUserMessage() const { return UserMessage; }
	std::string getLLMResponse() const { return LLMResponse; }

	void setId(uint32_t newId) { Id = newId; }
	void setTimestamp(uint64_t newTimestamp) { Timestamp = newTimestamp; }
	void setUserMessage(const std::string& newUserMessage) { UserMessage = newUserMessage; }
	void setLLMResponse(const std::string& newLLMResponse) { LLMResponse = newLLMResponse; }
};