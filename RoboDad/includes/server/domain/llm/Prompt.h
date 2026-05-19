#pragma once
#include "domain/IAggregateRoot.h"
#include "domain/llm/SystemInstructions.h"
#include "domain/llm/ChatHistory.h"
#include <cstdint>
#include <string>
#include <vector>

class Prompt : public IAggregateRoot<uint32_t> {
    uint32_t Id;
    SystemInstructions systemInstructions;
    std::vector<ChatHistory> chatHistories;
    std::string UserMessage;

public:
    Prompt(uint32_t id, const SystemInstructions& sysInstr,
           const std::vector<ChatHistory>& chatHist, const std::string& userMsg);

    uint32_t getId() const override;
    SystemInstructions getSystemInstructions() const;
    std::vector<ChatHistory> getChatHistories() const;
    std::string getUserMessage() const;

    void setId(uint32_t newId);
    void setSystemInstructions(const SystemInstructions& newSysInstr);
    void setChatHistories(const std::vector<ChatHistory>& newChatHist);
    void setUserMessage(const std::string& newUserMsg);
};
