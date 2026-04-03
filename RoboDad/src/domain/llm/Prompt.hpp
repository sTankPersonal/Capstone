#pragma once
#include <string>
#include <vector>
#include "SystemInstructions.hpp"
#include "ChatHistory.hpp"

class Prompt {
	uint32_t Id;
	SystemInstructions systemInstructions;
	std::vector<ChatHistory> chatHistories;
	std::string UserMessage;

public:
	Prompt(uint32_t id, const SystemInstructions& sysInstr, const std::vector<ChatHistory>& chatHist, const std::string& userMsg)
		: Id(id), systemInstructions(sysInstr), chatHistories(chatHist), UserMessage(userMsg) {
	}

	uint32_t getId() const { return Id; }
	SystemInstructions getSystemInstructions() const { return systemInstructions; }
	std::vector<ChatHistory> getChatHistories() const { return chatHistories; }
	std::string getUserMessage() const { return UserMessage; }

	void setId(uint32_t newId) { Id = newId; }
	void setSystemInstructions(const SystemInstructions& newSysInstr) { systemInstructions = newSysInstr; }
	void setChatHistories(const std::vector<ChatHistory>& newChatHist) { chatHistories = newChatHist; }
	void setUserMessage(const std::string& newUserMsg) { UserMessage = newUserMsg; }
};
