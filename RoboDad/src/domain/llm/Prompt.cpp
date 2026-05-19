#include "domain/llm/Prompt.h"

Prompt::Prompt(uint32_t id, const SystemInstructions& sysInstr,
               const std::vector<ChatHistory>& chatHist, const std::string& userMsg)
    : Id(id), systemInstructions(sysInstr), chatHistories(chatHist), UserMessage(userMsg) {}

uint32_t Prompt::getId() const { return Id; }
SystemInstructions Prompt::getSystemInstructions() const { return systemInstructions; }
std::vector<ChatHistory> Prompt::getChatHistories() const { return chatHistories; }
std::string Prompt::getUserMessage() const { return UserMessage; }

void Prompt::setId(uint32_t newId) { Id = newId; }
void Prompt::setSystemInstructions(const SystemInstructions& newSysInstr) { systemInstructions = newSysInstr; }
void Prompt::setChatHistories(const std::vector<ChatHistory>& newChatHist) { chatHistories = newChatHist; }
void Prompt::setUserMessage(const std::string& newUserMsg) { UserMessage = newUserMsg; }
