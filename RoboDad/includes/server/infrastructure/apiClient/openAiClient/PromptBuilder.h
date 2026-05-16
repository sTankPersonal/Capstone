#pragma once
#include "llm/Prompt.hpp"
#include "llm/ChatHistory.hpp"
#include "llm/SystemInstructions.hpp"
#include "budget/BudgetGoal.hpp"
#include "finance/Finance.hpp"
#include <string>
#include <vector>

// Fluent builder that assembles a Prompt domain object from system instructions,
// prior conversation history, and a user message optionally enriched with
// financial context.
class PromptBuilder {
    SystemInstructions instructions_;
    std::vector<ChatHistory> history_;
    std::string userMessage_;

public:
    // Sets the system instructions (persona, safety guidelines, overarching goal).
    PromptBuilder& withSystemInstructions(const SystemInstructions& instructions);

    // Appends prior conversation turns to provide the LLM with multi-turn context.
    PromptBuilder& withChatHistory(const std::vector<ChatHistory>& history);

    // Sets the raw text of the user's current message.
    PromptBuilder& withUserMessage(const std::string& message);

    // Formats the user's budget goals and expense list into a structured summary
    // and appends it to the user message so the LLM has concrete financial context.
    PromptBuilder& withBudgetContext(const std::vector<BudgetGoal>& goals,
                                     const std::vector<Finance>& expenses);

    // Assembles and returns the final Prompt domain object from the accumulated state.
    // Resets builder state after calling so the builder can be reused.
    Prompt build();

    // Returns the default RoboDad system instructions: a friendly financial advisor
    // persona focused on budget tracking, encouragement, and practical advice.
    static SystemInstructions defaultSystemInstructions();
};
