#pragma once
#include "infrastructureServices/persistance/BudgetRepository.h"
#include "infrastructureServices/persistance/ExpenseRepository.h"
#include "infrastructureServices/persistance/PromptRepository.h"
#include "infrastructureServices/persistance/ChatHistoryRepository.h"
#include "infrastructureServices/apiClient/ILlmClient.h"
#include "infrastructureServices/apiClient/IPromptBuilder.h"
#include <cstdint>
#include <string>

// Use case: generate AI-powered budget feedback for a user.
// Orchestrates data retrieval, prompt construction, LLM invocation,
// and persistence of the conversation exchange.
class GetBudgetFeedback {
    BudgetRepository&      budgetRepo_;
    ExpenseRepository&     expenseRepo_;
    PromptRepository&      promptRepo_;
    ChatHistoryRepository& chatHistoryRepo_;
    ILlmClient&            llmClient_;
    IPromptBuilder&        promptBuilder_;

public:
    GetBudgetFeedback(BudgetRepository&, ExpenseRepository&,
                      PromptRepository&, ChatHistoryRepository&,
                      ILlmClient&, IPromptBuilder&);

    // 1. Fetches the user's active budget goals and full expense history.
    // 2. Builds a Prompt via PromptBuilder (system instructions + prior history + context).
    // 3. Calls OpenAIClient::generate to get the LLM response.
    // 4. Persists the new ChatHistory entry and updated Prompt via their repositories.
    // 5. Returns the LLM response text to the caller.
    std::string execute(uint32_t userId, const std::string& userMessage);
};
