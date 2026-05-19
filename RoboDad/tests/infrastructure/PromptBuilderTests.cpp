#include <gtest/gtest.h>
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "domain/llm/SystemInstructions.h"
#include "domain/llm/ChatHistory.h"
#include "domain/budget/BudgetGoal.h"
#include "domain/finance/Finance.h"
#include "domain/finance/FinanceEnum.h"

// ── defaultSystemInstructions ─────────────────────────────────────────────────

TEST(PromptBuilderTest, DefaultSystemInstructionsAreNonEmpty) {
    auto instr = PromptBuilder::defaultSystemInstructions();
    EXPECT_FALSE(instr.getPersonality().empty());
    EXPECT_FALSE(instr.getSafetyGuidelines().empty());
    EXPECT_FALSE(instr.getGoal().empty());
}

TEST(PromptBuilderTest, DefaultSystemInstructionsAreStable) {
    // Two calls return equivalent content.
    auto a = PromptBuilder::defaultSystemInstructions();
    auto b = PromptBuilder::defaultSystemInstructions();
    EXPECT_EQ(a.getPersonality(),      b.getPersonality());
    EXPECT_EQ(a.getSafetyGuidelines(), b.getSafetyGuidelines());
    EXPECT_EQ(a.getGoal(),             b.getGoal());
}

// ── withUserMessage ───────────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithUserMessageSetsUserMessage) {
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("How am I doing?")
        .build();
    EXPECT_EQ(p.getUserMessage(), "How am I doing?");
}

TEST(PromptBuilderTest, EmptyUserMessageIsPreserved) {
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("")
        .build();
    EXPECT_TRUE(p.getUserMessage().empty());
}

// ── withSystemInstructions ────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithSystemInstructionsSetsAllFields) {
    SystemInstructions instr(0u, "TestPersona", "Safety rules", "Help users");
    Prompt p = PromptBuilder()
        .withSystemInstructions(instr)
        .withUserMessage("test")
        .build();
    EXPECT_EQ(p.getSystemInstructions().getPersonality(),      "TestPersona");
    EXPECT_EQ(p.getSystemInstructions().getSafetyGuidelines(), "Safety rules");
    EXPECT_EQ(p.getSystemInstructions().getGoal(),             "Help users");
}

// ── withChatHistory ───────────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithChatHistoryStoresSingleTurn) {
    ChatHistory h(1u, 1000u, "Hello", "Hi there!");
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withChatHistory({h})
        .withUserMessage("Follow-up")
        .build();
    ASSERT_EQ(p.getChatHistories().size(), 1u);
    EXPECT_EQ(p.getChatHistories()[0].getUserMessage(), "Hello");
    EXPECT_EQ(p.getChatHistories()[0].getLLMResponse(), "Hi there!");
}

TEST(PromptBuilderTest, WithChatHistoryPreservesOrder) {
    ChatHistory h1(1u, 100u, "First",  "Reply 1");
    ChatHistory h2(2u, 200u, "Second", "Reply 2");
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withChatHistory({h1, h2})
        .withUserMessage("Third")
        .build();
    ASSERT_EQ(p.getChatHistories().size(), 2u);
    EXPECT_EQ(p.getChatHistories()[0].getUserMessage(), "First");
    EXPECT_EQ(p.getChatHistories()[1].getUserMessage(), "Second");
}

TEST(PromptBuilderTest, BuildWithNoHistoryProducesEmptyChatHistories) {
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("First message ever")
        .build();
    EXPECT_TRUE(p.getChatHistories().empty());
}

// ── withBudgetContext ─────────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithBudgetContextAppendsGoalDataToUserMessage) {
    BudgetGoal goal(1u, 7u, 500.0, 200.0, "Emergency Fund");
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Am I on track?")
        .withBudgetContext({goal}, {})
        .build();

    const std::string& msg = p.getUserMessage();
    EXPECT_NE(msg.find("500"),            std::string::npos); // endGoal
    EXPECT_NE(msg.find("Emergency Fund"), std::string::npos); // description
}

TEST(PromptBuilderTest, WithBudgetContextAppendsExpenseDataToUserMessage) {
    Finance expense(1u, 45.0, "Groceries", FinanceEnum::Expense);
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Show expenses.")
        .withBudgetContext({}, {expense})
        .build();

    const std::string& msg = p.getUserMessage();
    EXPECT_NE(msg.find("45"),         std::string::npos); // amount
    EXPECT_NE(msg.find("Groceries"),  std::string::npos); // description
}

TEST(PromptBuilderTest, WithBudgetContextMultipleGoalsAllAppear) {
    BudgetGoal a(1u, 1u, 1000.0, 500.0, "Vacation");
    BudgetGoal b(2u, 1u,  500.0, 100.0, "Emergency Fund");
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Overview?")
        .withBudgetContext({a, b}, {})
        .build();

    const std::string& msg = p.getUserMessage();
    EXPECT_NE(msg.find("Vacation"),       std::string::npos);
    EXPECT_NE(msg.find("Emergency Fund"), std::string::npos);
}

TEST(PromptBuilderTest, WithEmptyBudgetContextDoesNotCrash) {
    Prompt p = PromptBuilder()
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Hello")
        .withBudgetContext({}, {})
        .build();
    EXPECT_FALSE(p.getUserMessage().empty());
}

// ── builder reuse ─────────────────────────────────────────────────────────────

TEST(PromptBuilderTest, BuilderIsReusableAfterBuild) {
    PromptBuilder builder;
    builder.withSystemInstructions(PromptBuilder::defaultSystemInstructions())
           .withUserMessage("First")
           .build();

    // Second build should start from a clean slate.
    Prompt p2 = builder
        .withSystemInstructions(PromptBuilder::defaultSystemInstructions())
        .withUserMessage("Second")
        .build();

    EXPECT_EQ(p2.getUserMessage(), "Second");
    EXPECT_TRUE(p2.getChatHistories().empty());
}
