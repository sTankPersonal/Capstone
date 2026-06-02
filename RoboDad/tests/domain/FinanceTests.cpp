#include <gtest/gtest.h>
#include "Transactions.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "TransactionId.h"
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

// ── TransactionAmount ─────────────────────────────────────────────────────────

TEST(TransactionAmountTest, ConstructorPreservesAmountAndCurrencyId) {
    TransactionAmount amount(std::optional<double>{49.99}, CurrencyId{"CAD"});
    ASSERT_TRUE(amount.getAmount().has_value());
    EXPECT_DOUBLE_EQ(amount.getAmount().value(), 49.99);
    ASSERT_TRUE(amount.getCurrencyId().has_value());
    EXPECT_EQ(amount.getCurrencyId().value().getId(), "CAD");
}

TEST(TransactionAmountTest, EmptyOptionalsYieldNoValues) {
    TransactionAmount amount(std::nullopt, std::nullopt);
    EXPECT_FALSE(amount.getAmount().has_value());
    EXPECT_FALSE(amount.getCurrencyId().has_value());
}

TEST(TransactionAmountTest, AmountCanBeZero) {
    TransactionAmount amount(std::optional<double>{0.0}, CurrencyId{"USD"});
    ASSERT_TRUE(amount.getAmount().has_value());
    EXPECT_DOUBLE_EQ(amount.getAmount().value(), 0.0);
}

// ── TransactionDescription ────────────────────────────────────────────────────

TEST(TransactionDescriptionTest, ConstructorPreservesDescription) {
    TransactionDescription desc(std::optional<std::string>{"Grocery run"});
    ASSERT_TRUE(desc.getDescription().has_value());
    EXPECT_EQ(desc.getDescription().value(), "Grocery run");
}

TEST(TransactionDescriptionTest, NulloptYieldsNoDescription) {
    TransactionDescription desc(std::nullopt);
    EXPECT_FALSE(desc.getDescription().has_value());
}

// ── Transaction ───────────────────────────────────────────────────────────────

TEST(TransactionTest, ConstructorPreservesAllFields) {
    TransactionAmount amount(std::optional<double>{150.0}, CurrencyId{"USD"});
    TransactionDescription desc(std::optional<std::string>{"Office supplies"});
    auto txDate = makeDate(2024, 5, 20);
    auto created = makeDate(2024, 5, 20);

    Transaction tx(
        TransactionId{"tx-1"}, UserId{"user-1"}, TransactionCategoryId{"cat-1"},
        amount, desc, txDate, created
    );

    EXPECT_EQ(tx.getId().getId(), "tx-1");
    EXPECT_EQ(tx.getUserId().getId(), "user-1");
    EXPECT_EQ(tx.getCategoryId().getId(), "cat-1");
    ASSERT_TRUE(tx.getAmount().getAmount().has_value());
    EXPECT_DOUBLE_EQ(tx.getAmount().getAmount().value(), 150.0);
    ASSERT_TRUE(tx.getAmount().getCurrencyId().has_value());
    EXPECT_EQ(tx.getAmount().getCurrencyId().value().getId(), "USD");
    ASSERT_TRUE(tx.getDescription().getDescription().has_value());
    EXPECT_EQ(tx.getDescription().getDescription().value(), "Office supplies");
    EXPECT_EQ(tx.getTransactionDate(), txDate);
    EXPECT_EQ(tx.getCreatedAt(), created);
}

TEST(TransactionTest, ConstructorAcceptsEmptyAmountAndDescription) {
    TransactionAmount amount(std::nullopt, std::nullopt);
    TransactionDescription desc(std::nullopt);
    auto date = makeDate(2024, 6, 1);

    Transaction tx(
        TransactionId{"tx-2"}, UserId{"user-2"}, TransactionCategoryId{"cat-2"},
        amount, desc, date, date
    );

    EXPECT_FALSE(tx.getAmount().getAmount().has_value());
    EXPECT_FALSE(tx.getDescription().getDescription().has_value());
}

TEST(TransactionTest, DistinctTransactionsHaveDistinctIds) {
    TransactionAmount amount(std::optional<double>{100.0}, CurrencyId{"USD"});
    TransactionDescription desc(std::nullopt);
    auto date = makeDate(2024, 1, 1);

    Transaction a(TransactionId{"tx-a"}, UserId{"user-1"}, TransactionCategoryId{"cat-1"}, amount, desc, date, date);
    Transaction b(TransactionId{"tx-b"}, UserId{"user-1"}, TransactionCategoryId{"cat-1"}, amount, desc, date, date);

    EXPECT_NE(a.getId().getId(), b.getId().getId());
}

TEST(TransactionTest, TransactionDatePreservedSeparatelyFromCreatedAt) {
    TransactionAmount amount(std::nullopt, std::nullopt);
    TransactionDescription desc(std::nullopt);
    auto txDate = makeDate(2024, 3, 15);
    auto created = makeDate(2024, 3, 16);

    Transaction tx(
        TransactionId{"tx-3"}, UserId{"user-1"}, TransactionCategoryId{"cat-1"},
        amount, desc, txDate, created
    );

    EXPECT_EQ(tx.getTransactionDate(), txDate);
    EXPECT_EQ(tx.getCreatedAt(), created);
    EXPECT_NE(tx.getTransactionDate(), tx.getCreatedAt());
}
