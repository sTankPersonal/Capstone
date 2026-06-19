#include <gtest/gtest.h>
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"

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

// ── withInsights ──────────────────────────────────────────────────────────────

TEST(PromptBuilderTest, WithInsightsAppendsSummaryContent) {
    FinancialInsightsDto insights;
    insights.totalIncome = 1000.0;
    insights.totalExpenses = 250.0;
    insights.expenseByCategoryList.push_back(CategoryItem{"Food", 250.0, 3});
    insights.unusualExpenses.push_back("Large grocery bill");

    std::string result = PromptBuilder()
        .withUserMessage("What did I spend?")
        .withInsights(insights)
        .build();

    EXPECT_NE(result.find("What did I spend?"), std::string::npos);
    EXPECT_NE(result.find("Total Expenses: $250"), std::string::npos);
    EXPECT_NE(result.find("Food: $250"), std::string::npos);
    EXPECT_NE(result.find("Large grocery bill"), std::string::npos);
}

TEST(PromptBuilderTest, WithInsightsIncludesIncomeAndExpenseSections) {
    FinancialInsightsDto insights;
    insights.totalIncome = 3500.0;
    insights.totalExpenses = 1800.0;

    std::string result = PromptBuilder()
        .withUserMessage("Summarize my spending.")
        .withInsights(insights)
        .build();

    EXPECT_NE(result.find("Income"), std::string::npos);
    EXPECT_NE(result.find("Expenses"), std::string::npos);
}

TEST(PromptBuilderTest, WithEmptyInsightsStillAddsSummaryHeaders) {
    FinancialInsightsDto insights;

    std::string result = PromptBuilder()
        .withUserMessage("Hello")
        .withInsights(insights)
        .build();

    EXPECT_NE(result.find("Hello"), std::string::npos);
    EXPECT_NE(result.find("Financial Summary"), std::string::npos);
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

TEST(PromptBuilderTest, SecondBuildDoesNotCarryOverPreviousInsights) {
    FinancialInsightsDto insights;
    insights.unusualExpenses.push_back("Subscription");

    PromptBuilder builder;
    builder.withUserMessage("First").withInsights(insights).build();

    std::string second = builder
        .withUserMessage("Clean slate")
        .build();

    EXPECT_EQ(second.find("Subscription"), std::string::npos);
}
