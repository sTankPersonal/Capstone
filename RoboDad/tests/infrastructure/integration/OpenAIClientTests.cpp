#include <gtest/gtest.h>
#include <cstdlib>
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "budget/BudgetGoal.hpp"
#include "finance/Finance.hpp"
#include "finance/FinanceEnum.hpp"
#include "llm/ChatHistory.hpp"

class OpenAIClientTest : public ::testing::Test {
protected:
    std::string apiKey_;
    std::string model_;

    void SetUp() override {
        apiKey_ = std::getenv("OPENAI_API_KEY");
        model_ = "gpt-4o-mini";

        if (!apiKey_ || apiKey_.empty()) {
            GTEST_SKIP() << "OPENAI_API_KEY not set; skipping integration tests.";
        }

    }
};

TEST_F(OpenAIClientTest, GenerateReturnsNonEmptyResponse) {
    OpenAIClient client(apiKey_, model_);
    Prompt prompt = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Say hello in one word.")
        .build();

    const std::string response = client.generate(prompt);
    EXPECT_FALSE(response.empty());
}

TEST_F(OpenAIClientTest, GenerateWithChatHistoryDoesNotCrash) {
    // Verifies that prior conversation turns are serialized without error.
    OpenAIClient client(apiKey_, model_);
    ChatHistory prior(0u, 0u, "My name is Alice.", "Nice to meet you, Alice!");
    Prompt prompt = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withChatHistory({prior})
        .withUserMessage("Repeat my name.")
        .build();

    EXPECT_NO_THROW({
        const std::string response = client.generate(prompt);
        EXPECT_FALSE(response.empty());
    });
}

TEST_F(OpenAIClientTest, GenerateWithBudgetContextDoesNotCrash) {
    // Verifies that financial context is serialized and sent correctly.
    OpenAIClient client(apiKey_, model_);
    BudgetGoal goal(0u, 1u, 1000.0, 200.0, "Emergency Fund");
    Finance    expense(0u, 500.0, "Rent", FinanceEnum::Expense);

    Prompt prompt = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withBudgetContext({goal}, {expense})
        .withUserMessage("Am I on track with my emergency fund?")
        .build();

    EXPECT_NO_THROW({
        const std::string response = client.generate(prompt);
        EXPECT_FALSE(response.empty());
    });
}
