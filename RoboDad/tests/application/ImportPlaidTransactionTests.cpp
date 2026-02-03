#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ITransactionRepository.h"
#include "ITransactionCategoryRepository.h"
#include "ICurrencyRepository.h"
#include "IPlaidClient.h"
#include "ImportPlaidTransactions.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "UserId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "TransactionCategory.h"
#include "Currency.h"
#include "PlaidTransactionData.h"
#include <chrono>

using ::testing::_;
using ::testing::Return;

namespace {

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

static Transaction makeTx(const std::string& id, const std::string& userId) {
    auto d = makeDate(2024, 1, 1);
    return Transaction(
        TransactionId{id}, UserId{userId}, TransactionCategoryId{"other"},
        TransactionAmount(std::optional<double>{10.0}, std::optional<CurrencyId>{}),
        TransactionDescription(std::optional<std::string>{"Test"}),
        d, d
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

class MockTransactionCategoryRepository : public ITransactionCategoryRepository {
public:
    MOCK_METHOD(TransactionCategory,                create,      (const TransactionCategory&), (override));
    MOCK_METHOD(std::optional<TransactionCategory>, findById,    (TransactionCategoryId),      (override));
    MOCK_METHOD(std::vector<TransactionCategory>,   findAll,     (),                           (override));
    MOCK_METHOD(bool,                               update,      (const TransactionCategory&), (override));
    MOCK_METHOD(bool,                               remove,      (TransactionCategoryId),      (override));
    MOCK_METHOD(std::optional<TransactionCategory>, findByValue, (const std::string&),         (override));
};

class MockCurrencyRepository : public ICurrencyRepository {
public:
    MOCK_METHOD(Currency,                create,      (const Currency&),    (override));
    MOCK_METHOD(std::optional<Currency>, findById,    (CurrencyId),         (override));
    MOCK_METHOD(std::vector<Currency>,   findAll,     (),                   (override));
    MOCK_METHOD(bool,                    update,      (const Currency&),    (override));
    MOCK_METHOD(bool,                    remove,      (CurrencyId),         (override));
    MOCK_METHOD(std::optional<Currency>, findByValue, (const std::string&), (override));
};

class MockPlaidClient : public IPlaidClient {
public:
    MOCK_METHOD(std::string, createSandboxAccessToken,
                (const std::string&), (override));
    MOCK_METHOD(std::vector<PlaidTransactionData>, fetchTransactions,
                (const std::string&, const std::string&, const std::string&, int), (override));
};

} // namespace

// ── ImportPlaidTransactions ───────────────────────────────────────────────────

TEST(ImportPlaidTransactionsTest, ExecuteCreatesOneTransactionPerPlaidRecord) {
    MockTransactionRepository         txRepo;
    MockTransactionCategoryRepository catRepo;
    MockCurrencyRepository            curRepo;
    MockPlaidClient                   plaid;

    PlaidTransactionData r1{45.0, "Groceries", "2024-03-01", "Food",   "USD"};
    PlaidTransactionData r2{12.5, "Bus pass",  "2024-03-02", "Travel", "USD"};

    EXPECT_CALL(plaid, createSandboxAccessToken(_)).WillOnce(Return("tok-sandbox"));
    EXPECT_CALL(plaid, fetchTransactions("tok-sandbox", "2024-03-01", "2024-03-31", 100))
        .WillOnce(Return(std::vector<PlaidTransactionData>{r1, r2}));

    EXPECT_CALL(catRepo, findByValue("Food"))
        .WillOnce(Return(std::optional<TransactionCategory>{
            TransactionCategory(TransactionCategoryId{"cat-food"}, "Food", makeDate(2024,1,1))}));
    EXPECT_CALL(catRepo, findByValue("Travel"))
        .WillOnce(Return(std::optional<TransactionCategory>{
            TransactionCategory(TransactionCategoryId{"cat-travel"}, "Travel", makeDate(2024,1,1))}));

    EXPECT_CALL(curRepo, findByValue("USD"))
        .Times(2)
        .WillRepeatedly(Return(std::optional<Currency>{
            Currency(CurrencyId{"USD"}, "USD", makeDate(2024,1,1))}));

    EXPECT_CALL(txRepo, create(_))
        .WillOnce(Return(makeTx("tx-1", "user-1")))
        .WillOnce(Return(makeTx("tx-2", "user-1")));

    ImportPlaidTransactions useCase(txRepo, catRepo, curRepo, plaid);
    auto result = useCase.execute(UserId{"user-1"}, "2024-03-01", "2024-03-31");

    ASSERT_EQ(result.size(), 2u);
}

TEST(ImportPlaidTransactionsTest, ExecuteFallsBackToOtherCategoryWhenCategoryNotFound) {
    MockTransactionRepository         txRepo;
    MockTransactionCategoryRepository catRepo;
    MockCurrencyRepository            curRepo;
    MockPlaidClient                   plaid;

    PlaidTransactionData r{99.0, "Mystery", "2024-04-01", "UnknownCategory", "USD"};

    EXPECT_CALL(plaid, createSandboxAccessToken(_)).WillOnce(Return("tok-sandbox"));
    EXPECT_CALL(plaid, fetchTransactions(_, _, _, _))
        .WillOnce(Return(std::vector<PlaidTransactionData>{r}));
    EXPECT_CALL(catRepo, findByValue("UnknownCategory")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(curRepo, findByValue(_))
        .WillOnce(Return(std::optional<Currency>{
            Currency(CurrencyId{"USD"}, "USD", makeDate(2024,1,1))}));

    EXPECT_CALL(txRepo, create(_)).WillOnce(Return(makeTx("tx-1", "user-1")));

    ImportPlaidTransactions useCase(txRepo, catRepo, curRepo, plaid);
    auto result = useCase.execute(UserId{"user-1"}, "2024-04-01", "2024-04-30");

    ASSERT_EQ(result.size(), 1u);
}

TEST(ImportPlaidTransactionsTest, ExecuteSetsNulloptCurrencyWhenCurrencyNotFound) {
    MockTransactionRepository         txRepo;
    MockTransactionCategoryRepository catRepo;
    MockCurrencyRepository            curRepo;
    MockPlaidClient                   plaid;

    PlaidTransactionData r{20.0, "Coffee", "2024-04-05", "Food", "XYZ"};

    EXPECT_CALL(plaid, createSandboxAccessToken(_)).WillOnce(Return("tok-sandbox"));
    EXPECT_CALL(plaid, fetchTransactions(_, _, _, _))
        .WillOnce(Return(std::vector<PlaidTransactionData>{r}));
    EXPECT_CALL(catRepo, findByValue("Food"))
        .WillOnce(Return(std::optional<TransactionCategory>{
            TransactionCategory(TransactionCategoryId{"cat-food"}, "Food", makeDate(2024,1,1))}));
    EXPECT_CALL(curRepo, findByValue("XYZ")).WillOnce(Return(std::nullopt));

    EXPECT_CALL(txRepo, create(_)).WillOnce(Return(makeTx("tx-1", "user-1")));

    ImportPlaidTransactions useCase(txRepo, catRepo, curRepo, plaid);
    auto result = useCase.execute(UserId{"user-1"}, "2024-04-01", "2024-04-30");

    ASSERT_EQ(result.size(), 1u);
}

TEST(ImportPlaidTransactionsTest, ExecuteReturnsEmptyWhenPlaidHasNoTransactions) {
    MockTransactionRepository         txRepo;
    MockTransactionCategoryRepository catRepo;
    MockCurrencyRepository            curRepo;
    MockPlaidClient                   plaid;

    EXPECT_CALL(plaid, createSandboxAccessToken(_)).WillOnce(Return("tok-sandbox"));
    EXPECT_CALL(plaid, fetchTransactions(_, _, _, _))
        .WillOnce(Return(std::vector<PlaidTransactionData>{}));

    ImportPlaidTransactions useCase(txRepo, catRepo, curRepo, plaid);
    auto result = useCase.execute(UserId{"user-1"}, "2024-05-01", "2024-05-31");

    EXPECT_TRUE(result.empty());
}
