#include "infrastructure/ai/PromptBuilder.h"

PromptBuilder& PromptBuilder::withSystemInstructions(const SystemInstructions& instructions) {
    // TODO: assign instructions to instructions_; return *this
}

PromptBuilder& PromptBuilder::withChatHistory(const std::vector<ChatHistory>& history) {
    // TODO: assign history to history_; return *this
}

PromptBuilder& PromptBuilder::withUserMessage(const std::string& message) {
    // TODO: assign message to userMessage_; return *this
}

PromptBuilder& PromptBuilder::withBudgetContext(const std::vector<BudgetGoal>& goals,
                                                const std::vector<Finance>& expenses) {
    // TODO: format goals into a human-readable string
    //       (e.g. "Budget Goals:\n- Save $500 for vacation by ...\n");
    //       format expenses into a summary string
    //       (e.g. "Recent Expenses:\n- $45.00 Groceries (Expense)\n");
    //       append both summaries to userMessage_;
    //       return *this
}

Prompt PromptBuilder::build() {
    // TODO: construct and return a Prompt from instructions_, history_, and userMessage_;
    //       reset instructions_, history_, and userMessage_ to defaults so the builder
    //       can be safely reused
}

SystemInstructions PromptBuilder::defaultSystemInstructions() {
    // TODO: construct and return a SystemInstructions with:
    //         personality:       "RoboDad — a warm, encouraging, no-nonsense financial advisor"
    //         safetyGuidelines:  "Never give investment advice; always recommend consulting
    //                             a certified financial planner for major decisions"
    //         goal:              "Help the user understand their budget, track expenses,
    //                             and make practical progress toward their financial goals"
}
