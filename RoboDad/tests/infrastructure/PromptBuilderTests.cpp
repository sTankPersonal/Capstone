#include <gtest/gtest.h>
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include <chrono>

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

static Transaction makeTransaction(const std::string& id, double amount, const std::string& desc) {
    auto date = makeDate(2024, 1, 1);
    return Transaction(
        TransactionId{id}, UserId{"user-1"}, TransactionCategoryId{"cat-1"},
        TransactionAmount(std::optional<double>{amount}, CurrencyId{"USD"}),
        TransactionDescription(std::optional<std::string>{desc}),
        date, date
    );
}

// ── withUserMessage ───────────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithUserMessageSetsMessage) {
    std::string result = PromptBuilder()
        .withUserMessage("How am I doing?")
        .build();
    EXPECT_EQ(result, "How am I doing?");
}

TEST(PromptBuilderTest, EmptyUserMessageIsPreserved) {
    std::string result = PromptBuilder()
        .withUserMessage("")
        .build();
    EXPECT_TRUE(result.empty());
}

// ── withTransactionContext ────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithTransactionContextAppendsAmountAndDescription) {
    Transaction tx = makeTransaction("tx-1", 45.0, "Groceries");
    std::string result = PromptBuilder()
        .withUserMessage("What did I spend?")
        .withTransactionContext({tx})
        .build();
    EXPECT_NE(result.find("45"),        std::string::npos);
    EXPECT_NE(result.find("Groceries"), std::string::npos);
}

TEST(PromptBuilderTest, WithTransactionContextMultipleTransactionsAllAppear) {
    Transaction a = makeTransaction("tx-a", 1000.0, "Rent");
    Transaction b = makeTransaction("tx-b",   25.5, "Coffee");
    std::string result = PromptBuilder()
        .withUserMessage("Summarize my spending.")
        .withTransactionContext({a, b})
        .build();
    EXPECT_NE(result.find("Rent"),   std::string::npos);
    EXPECT_NE(result.find("Coffee"), std::string::npos);
}

TEST(PromptBuilderTest, WithEmptyTransactionContextDoesNotAlterMessage) {
    std::string result = PromptBuilder()
        .withUserMessage("Hello")
        .withTransactionContext({})
        .build();
    EXPECT_EQ(result, "Hello");
}

// ── build ─────────────────────────────────────────────────────────────────────

TEST(PromptBuilderTest, BuildReturnsNonEmptyStringWhenMessageSet) {
    std::string result = PromptBuilder()
        .withUserMessage("Test message")
        .build();
    EXPECT_FALSE(result.empty());
}

TEST(PromptBuilderTest, BuilderIsReusableAfterBuild) {
    PromptBuilder builder;
    builder.withUserMessage("First").build();

    std::string second = builder
        .withUserMessage("Second")
        .build();

    EXPECT_EQ(second, "Second");
}

TEST(PromptBuilderTest, SecondBuildDoesNotCarryOverPreviousTransactions) {
    Transaction tx = makeTransaction("tx-1", 99.0, "Subscription");
    PromptBuilder builder;
    builder.withUserMessage("First").withTransactionContext({tx}).build();

    std::string second = builder
        .withUserMessage("Clean slate")
        .build();

    EXPECT_EQ(second.find("Subscription"), std::string::npos);
}
