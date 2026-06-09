#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ITransactionRepository.h"
#include "CreateTransaction.h"
#include "ListTransactions.h"
#include "ListTransactionsByCategory.h"
#include "GetTransaction.h"
#include "UpdateTransaction.h"
#include "DeleteTransaction.h"
#include "CreateTransactionCommand.h"
#include "UpdateTransactionCommand.h"
#include "DeleteTransactionCommand.h"
#include "ListTransactionsQuery.h"
#include "GetTransactionQuery.h"
#include "ListTransactionsByCategoryQuery.h"
#include "TransactionDto.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include <chrono>

using ::testing::Return;
using ::testing::_;

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

static Transaction makeTransaction(const std::string& id, const std::string& userId,
                                   double amount = 100.0, const std::string& desc = "Test",
                                   const std::string& categoryId = "cat-1") {
    auto date = makeDate(2024, 1, 1);
    return Transaction(
        TransactionId{id}, UserId{userId}, TransactionCategoryId{categoryId},
        TransactionAmount(std::optional<double>{amount}, std::optional<CurrencyId>{CurrencyId{"USD"}}),
        TransactionDescription(std::optional<std::string>{desc}),
        date, date
    );
}

class MockTransactionRepository : public ITransactionRepository {
public:
    MOCK_METHOD(Transaction,                create,       (const Transaction&), (override));
    MOCK_METHOD(std::optional<Transaction>, findById,     (TransactionId),      (override));
    MOCK_METHOD(std::vector<Transaction>,   findAll,      (),                   (override));
    MOCK_METHOD(bool,                       update,       (const Transaction&), (override));
    MOCK_METHOD(bool,                       remove,       (TransactionId),      (override));
    MOCK_METHOD(std::vector<Transaction>,   findByUserId, (const UserId&),      (override));
};

// ── CreateTransaction ─────────────────────────────────────────────────────────

TEST(CreateTransactionTest, ExecuteCreatesTransactionAndReturnsResult) {
    MockTransactionRepository repo;
    Transaction expected = makeTransaction("tx-1", "user-1", 49.99, "Grocery run");
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    CreateTransaction useCase(repo);
    TransactionDto result = useCase.execute(
        CreateTransactionCommand{
            UserId{"user-1"}, TransactionCategoryId{"cat-1"},
            49.99, std::optional<CurrencyId>{CurrencyId{"USD"}}, "Grocery run",
            makeDate(2024, 5, 1)
        }
    );

    EXPECT_EQ(result.getUserId(), "user-1");
}

TEST(CreateTransactionTest, ExecuteAcceptsNoCurrency) {
    MockTransactionRepository repo;
    Transaction expected = makeTransaction("tx-2", "user-2", 25.0, "Cash purchase");
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    CreateTransaction useCase(repo);
    TransactionDto result = useCase.execute(
        CreateTransactionCommand{
            UserId{"user-2"}, TransactionCategoryId{"cat-1"},
            25.0, std::nullopt, "Cash purchase",
            makeDate(2024, 6, 15)
        }
    );

    EXPECT_EQ(result.getUserId(), "user-2");
}

// ── ListTransactions ──────────────────────────────────────────────────────────

TEST(ListTransactionsTest, ExecuteReturnsAllTransactionsForUser) {
    MockTransactionRepository repo;
    std::vector<Transaction> txns = {
        makeTransaction("tx-a", "user-1", 50.0, "Groceries"),
        makeTransaction("tx-b", "user-1", 12.50, "Bus pass"),
    };
    EXPECT_CALL(repo, findByUserId(UserId{"user-1"})).WillOnce(Return(txns));

    ListTransactions useCase(repo);
    auto result = useCase.execute(ListTransactionsQuery{UserId{"user-1"}});

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getUserId(), "user-1");
    EXPECT_EQ(result[1].getUserId(), "user-1");
}

TEST(ListTransactionsTest, ExecuteReturnsEmptyVectorForNewUser) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, findByUserId(UserId{"user-99"}))
        .WillOnce(Return(std::vector<Transaction>{}));

    ListTransactions useCase(repo);
    EXPECT_TRUE(useCase.execute(ListTransactionsQuery{UserId{"user-99"}}).empty());
}

// ── GetTransaction ────────────────────────────────────────────────────────────

TEST(GetTransactionTest, ExecuteReturnsTransactionWhenFound) {
    MockTransactionRepository repo;
    Transaction expected = makeTransaction("tx-1", "user-1");
    EXPECT_CALL(repo, findById(TransactionId{"tx-1"}))
        .WillOnce(Return(std::optional<Transaction>{expected}));

    GetTransaction useCase(repo);
    auto result = useCase.execute(GetTransactionQuery{TransactionId{"tx-1"}});

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getUserId(), "user-1");
}

TEST(GetTransactionTest, ExecuteReturnsNulloptWhenNotFound) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, findById(TransactionId{"missing"}))
        .WillOnce(Return(std::nullopt));

    GetTransaction useCase(repo);
    EXPECT_FALSE(useCase.execute(GetTransactionQuery{TransactionId{"missing"}}).has_value());
}

// ── UpdateTransaction ─────────────────────────────────────────────────────────

TEST(UpdateTransactionTest, ExecuteReturnsTrueOnSuccess) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, findById(TransactionId{"tx-1"}))
        .WillOnce(Return(std::optional<Transaction>{makeTransaction("tx-1", "user-1")}));
    EXPECT_CALL(repo, update(_)).WillOnce(Return(true));

    UpdateTransaction useCase(repo);
    EXPECT_TRUE(useCase.execute(
        UpdateTransactionCommand{
            TransactionId{"tx-1"}, TransactionCategoryId{"cat-2"},
            200.0, std::optional<CurrencyId>{CurrencyId{"CAD"}}, "Updated description",
            makeDate(2024, 6, 1)
        }
    ));
}

TEST(UpdateTransactionTest, ExecuteReturnsFalseForMissingTransaction) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, findById(TransactionId{"missing"}))
        .WillOnce(Return(std::nullopt));

    UpdateTransaction useCase(repo);
    EXPECT_FALSE(useCase.execute(
        UpdateTransactionCommand{
            TransactionId{"missing"}, TransactionCategoryId{"cat-1"},
            100.0, std::nullopt, "Any description",
            makeDate(2024, 1, 1)
        }
    ));
}

// ── DeleteTransaction ─────────────────────────────────────────────────────────

TEST(DeleteTransactionTest, ExecuteReturnsTrueOnSuccess) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, remove(TransactionId{"tx-1"})).WillOnce(Return(true));

    DeleteTransaction useCase(repo);
    EXPECT_TRUE(useCase.execute(DeleteTransactionCommand{TransactionId{"tx-1"}}));
}

TEST(DeleteTransactionTest, ExecuteReturnsFalseForMissingTransaction) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, remove(TransactionId{"missing"})).WillOnce(Return(false));

    DeleteTransaction useCase(repo);
    EXPECT_FALSE(useCase.execute(DeleteTransactionCommand{TransactionId{"missing"}}));
}

// ── ListTransactionsByCategory ────────────────────────────────────────────────

TEST(ListTransactionsByCategoryTest, ExecuteReturnsOnlyMatchingCategory) {
    MockTransactionRepository repo;
    std::vector<Transaction> all = {
        makeTransaction("tx-a", "user-1", 50.0,  "Groceries", "cat-food"),
        makeTransaction("tx-b", "user-1", 80.0,  "Gas",       "cat-travel"),
        makeTransaction("tx-c", "user-1", 120.0, "Restaurant","cat-food"),
    };
    EXPECT_CALL(repo, findByUserId(UserId{"user-1"})).WillOnce(Return(all));

    ListTransactionsByCategory useCase(repo);
    auto result = useCase.execute(ListTransactionsByCategoryQuery{UserId{"user-1"}, TransactionCategoryId{"cat-food"}});

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getCategoryId(), "cat-food");
    EXPECT_EQ(result[1].getCategoryId(), "cat-food");
}

TEST(ListTransactionsByCategoryTest, ExecuteReturnsEmptyWhenNoCategoryMatch) {
    MockTransactionRepository repo;
    std::vector<Transaction> all = {
        makeTransaction("tx-a", "user-1", 50.0, "Groceries", "cat-food"),
    };
    EXPECT_CALL(repo, findByUserId(UserId{"user-1"})).WillOnce(Return(all));

    ListTransactionsByCategory useCase(repo);
    auto result = useCase.execute(ListTransactionsByCategoryQuery{UserId{"user-1"}, TransactionCategoryId{"cat-travel"}});

    EXPECT_TRUE(result.empty());
}

TEST(ListTransactionsByCategoryTest, ExecuteReturnsEmptyForUserWithNoTransactions) {
    MockTransactionRepository repo;
    EXPECT_CALL(repo, findByUserId(UserId{"user-99"}))
        .WillOnce(Return(std::vector<Transaction>{}));

    ListTransactionsByCategory useCase(repo);
    auto result = useCase.execute(ListTransactionsByCategoryQuery{UserId{"user-99"}, TransactionCategoryId{"cat-food"}});

    EXPECT_TRUE(result.empty());
}
