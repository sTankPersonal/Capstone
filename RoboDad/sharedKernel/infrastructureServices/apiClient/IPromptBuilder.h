#pragma once
#include <string>
#include <vector>
#include "llm/Prompt.hpp"
#include "llm/ChatHistory.hpp"
#include "llm/SystemInstructions.hpp"
#include "budget/BudgetGoal.hpp"
#include "finance/Finance.hpp"

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
