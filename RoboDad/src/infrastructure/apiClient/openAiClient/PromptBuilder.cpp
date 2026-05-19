#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include <sstream>

PromptBuilder& PromptBuilder::withSystemInstructions(const SystemInstructions& instructions) {
    instructions_ = instructions;
    return *this;
}

PromptBuilder& PromptBuilder::withChatHistory(const std::vector<ChatHistory>& history) {
    history_ = history;
    return *this;
}

PromptBuilder& PromptBuilder::withUserMessage(const std::string& message) {
    userMessage_ = message;
    return *this;
}

PromptBuilder& PromptBuilder::withBudgetContext(const std::vector<BudgetGoal>& goals,
                                                const std::vector<Finance>& expenses) {
    std::ostringstream oss;

    if (!goals.empty()) {
        oss << "\nBudget Goals:\n";
        for (const auto& g : goals) {
            oss << "- " << g.getDescription()
                << ": $" << g.getAmount() << " saved of $" << g.getEndGoal() << " goal\n";
        }
    }

    if (!expenses.empty()) {
        oss << "\nRecent Expenses:\n";
        for (const auto& e : expenses) {
            oss << "- $" << e.getAmount() << " " << e.getDescription() << "\n";
        }
    }

    userMessage_ += oss.str();
    return *this;
}

Prompt PromptBuilder::build() {
    // If no system instructions were provided, use defaults
    if (instructions_.getPersonality().empty()) {
        instructions_ = defaultSystemInstructions();
    }

    Prompt p(0, instructions_, history_, userMessage_);

    // Reset builder state so it can be reused
    instructions_ = SystemInstructions();
    history_.clear();
    userMessage_.clear();

    return p;
}

SystemInstructions PromptBuilder::defaultSystemInstructions() {
    return SystemInstructions(
        0,
        "RoboDad — a warm, encouraging, no-nonsense financial advisor.",
        "Never give investment advice; always recommend consulting a certified financial planner for major decisions.",
        "Help the user understand their budget, track expenses, and make practical progress toward their financial goals."
    );

}
