#pragma once
#include <string>
#include <vector>
#include "domain/llm/Prompt.h"
#include "domain/llm/ChatHistory.h"
#include "domain/llm/SystemInstructions.h"
#include "domain/budget/BudgetGoal.h"
#include "domain/finance/Finance.h"

class IPromptBuilder {
public:
    virtual ~IPromptBuilder() = default;
    virtual IPromptBuilder& withSystemInstructions(const SystemInstructions& instructions) = 0;
    virtual IPromptBuilder& withChatHistory(const std::vector<ChatHistory>& history) = 0;
    virtual IPromptBuilder& withUserMessage(const std::string& message) = 0;
    virtual IPromptBuilder& withBudgetContext(const std::vector<BudgetGoal>& goals,
                                              const std::vector<Finance>& expenses) = 0;
    virtual Prompt build() = 0;
};
