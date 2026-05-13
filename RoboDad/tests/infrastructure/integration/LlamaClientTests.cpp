#include <gtest/gtest.h>
#include <cstdlib>
#include "infrastructure/ai/LlamaClient.h"
#include "infrastructure/ai/PromptBuilder.h"
#include "budget/BudgetGoal.hpp"
#include "finance/Finance.hpp"
#include "finance/FinanceEnum.hpp"
#include "llm/ChatHistory.hpp"

// These tests require a running Ollama instance.
// Set OLLAMA_URL (e.g. "http://localhost:11434") to enable them.
// If OLLAMA_URL is unset the entire suite is skipped via GTEST_SKIP().
// Optionally set OLLAMA_MODEL to override the default "llama3".
class LlamaClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* url   = std::getenv("OLLAMA_URL");
        const char* model = std::getenv("OLLAMA_MODEL");
        if (!url)
            GTEST_SKIP() << "OLLAMA_URL not set — skipping LlamaClient integration tests";
        baseUrl_ = url;
        model_   = model ? model : "llama3";
    }

    std::string baseUrl_;
    std::string model_;
};

TEST_F(LlamaClientTest, GenerateReturnsNonEmptyResponse) {
    LlamaClient client(baseUrl_, model_);
    Prompt prompt = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Say hello in one word.")
        .build();

    const std::string response = client.generate(prompt);
    EXPECT_FALSE(response.empty());
}

TEST_F(LlamaClientTest, GenerateWithChatHistoryDoesNotCrash) {
    // Verifies that prior conversation turns are serialized without error.
    LlamaClient client(baseUrl_, model_);
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

TEST_F(LlamaClientTest, GenerateWithBudgetContextDoesNotCrash) {
    // Verifies that financial context is serialized and sent correctly.
    LlamaClient client(baseUrl_, model_);
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

TEST_F(LlamaClientTest, GenerateThrowsOnUnreachableHost) {
    // A bad URL should produce a std::runtime_error from the curl layer.
    LlamaClient client("http://localhost:19999", model_);
    Prompt prompt = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("ping")
        .build();

    EXPECT_THROW(client.generate(prompt), std::runtime_error);
}
