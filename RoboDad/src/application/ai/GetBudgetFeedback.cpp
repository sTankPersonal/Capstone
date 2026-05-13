#include "application/ai/GetBudgetFeedback.h"

GetBudgetFeedback::GetBudgetFeedback(BudgetRepository& br, ExpenseRepository& er,
                                     PromptRepository& pr, ChatHistoryRepository& chr,
                                     LlamaClient& lc, PromptBuilder& pb)
    : budgetRepo_(br), expenseRepo_(er),
      promptRepo_(pr), chatHistoryRepo_(chr),
      llamaClient_(lc), promptBuilder_(pb) {}

std::string GetBudgetFeedback::execute(uint32_t userId, const std::string& userMessage) {
    // TODO 1 — Fetch context
    //   Load the user's budget goals:   budgetRepo_.findByUserId(userId)
    //   Load the user's expense history: expenseRepo_.findByUserId(userId)
    //   Load the user's prior prompt (if any): promptRepo_.findByUserId(userId)
    //   If a prior prompt exists, load its ChatHistory entries from chatHistoryRepo_

    // TODO 2 — Build the prompt
    //   Call promptBuilder_
    //       .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
    //       .withChatHistory(priorHistory)      // empty vector if first conversation
    //       .withBudgetContext(goals, expenses)
    //       .withUserMessage(userMessage)
    //       .build()

    // TODO 3 — Call the LLM
    //   Pass the built Prompt to llamaClient_.generate(prompt);
    //   capture the returned response string

    // TODO 4 — Persist the exchange
    //   Construct a ChatHistory value object from userMessage and the LLM response,
    //   with the current UTC timestamp
    //   If no prior Prompt record exists, create one via promptRepo_.create(userId, prompt);
    //   Save the new ChatHistory entry via chatHistoryRepo_.create(promptId, chatHistory)

    // TODO 5 — Return
    //   Return the LLM response string to the caller
}
