#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "ITransactionRepository.h"
#include "ICurrencyRepository.h"
#include "IPlaidClient.h"
#include "IPlaidItemRepository.h"
#include "ImportPlaidTransactions.h"
#include "ImportPlaidTransactionsCommand.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "UserId.h"
#include "PlaidItemId.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "Currency.h"
#include "PlaidItem.h"
#include "PlaidSyncResult.h"
#include "PlaidTransactionData.h"
#include <chrono>

using ::testing::_;
using ::testing::Return;

namespace {

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

static Transaction makeTx(const std::string& id, const std::string& userId,
                           const std::string& catId = "expenses") {
    auto d = makeDate(2024, 1, 1);
    return Transaction(
        TransactionId{id}, UserId{userId}, TransactionCategoryId{catId},
        TransactionAmount(std::optional<double>{10.0}, std::optional<CurrencyId>{}),
        TransactionDescription(std::optional<std::string>{"Test"}),
        d, d
    );
}

class MockTransactionRepository : public ITransactionRepository {
public:
    MOCK_METHOD(Transaction,                create,                    (const Transaction&),       (override));
    MOCK_METHOD(std::optional<Transaction>, findById,                  (TransactionId),             (override));
    MOCK_METHOD(std::vector<Transaction>,   findAll,                   (),                          (override));
    MOCK_METHOD(bool,                       update,                    (const Transaction&),        (override));
    MOCK_METHOD(bool,                       remove,                    (TransactionId),             (override));
    MOCK_METHOD(std::vector<Transaction>,   findByUserId,              (const UserId&),             (override));
    MOCK_METHOD(std::optional<Transaction>, findByPlaidTransactionId,  (const std::string&),        (override));
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
    MOCK_METHOD(std::string,     createLinkToken,         (const std::string&),               (override));
    MOCK_METHOD(std::string,     exchangePublicToken,      (const std::string&),               (override));
    MOCK_METHOD(std::string,     createSandboxAccessToken, (const std::string&),               (override));
    MOCK_METHOD(PlaidSyncResult, fetchTransactions,        (const std::string&, const std::string&), (override));
};

class MockPlaidItemRepository : public IPlaidItemRepository {
public:
    MOCK_METHOD(PlaidItem,                create,        (const PlaidItem&),  (override));
    MOCK_METHOD(std::optional<PlaidItem>, findById,      (PlaidItemId),       (override));
    MOCK_METHOD(std::vector<PlaidItem>,   findAll,       (),                  (override));
    MOCK_METHOD(bool,                     update,        (const PlaidItem&),  (override));
    MOCK_METHOD(bool,                     remove,        (PlaidItemId),       (override));
    MOCK_METHOD(std::optional<PlaidItem>, findByUserId,  (const UserId&),     (override));
};

static ImportPlaidTransactionsCommand makeCmd(const std::string& token = "tok-sandbox") {
    return ImportPlaidTransactionsCommand{
        UserId{"user-1"}, PlaidItemId{"item-1"}, token
    };
}

} // namespace

// ── ImportPlaidTransactions ───────────────────────────────────────────────────

TEST(ImportPlaidTransactionsTest, ExecuteCreatesOneTransactionPerAddedRecord) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData r1{"ptx-1", 45.0, "Groceries", "2024-03-01", "Food",   "USD"};
    PlaidTransactionData r2{"ptx-2", 12.5, "Bus pass",  "2024-03-02", "Travel", "USD"};

    PlaidSyncResult syncResult;
    syncResult.added = {r1, r2};
    syncResult.nextCursor = "cursor-abc";

    EXPECT_CALL(plaid, fetchTransactions("tok-sandbox", ""))
        .WillOnce(Return(syncResult));

    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-1")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-2")).WillOnce(Return(std::nullopt));

    EXPECT_CALL(curRepo, findByValue("USD"))
        .Times(2)
        .WillRepeatedly(Return(std::optional<Currency>{
            Currency(CurrencyId{"USD"}, "USD", makeDate(2024,1,1))}));

    EXPECT_CALL(txRepo, create(_))
        .WillOnce(Return(makeTx("tx-1", "user-1")))
        .WillOnce(Return(makeTx("tx-2", "user-1")));

    auto item = PlaidItem{PlaidItemId{"item-1"}, UserId{"user-1"}, "tok-sandbox", makeDate(2024,1,1)};
    EXPECT_CALL(plaidItemRepo, findById(PlaidItemId{"item-1"})).WillOnce(Return(item));
    EXPECT_CALL(plaidItemRepo, update(_)).WillOnce(Return(true));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    auto result = useCase.execute(makeCmd());

    ASSERT_EQ(result.size(), 2u);
}

TEST(ImportPlaidTransactionsTest, PositiveAmountMapsToExpenses) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData r{"ptx-3", 50.0, "Coffee", "2024-04-01", "Food", "USD"};
    PlaidSyncResult syncResult;
    syncResult.added = {r};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-3")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(curRepo, findByValue(_)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    std::string capturedCatId;
    EXPECT_CALL(txRepo, create(_))
        .WillOnce([&capturedCatId](const Transaction& tx) {
            capturedCatId = tx.getCategoryId().getId();
            return tx;
        });

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    useCase.execute(makeCmd());

    EXPECT_EQ(capturedCatId, "expenses");
}

TEST(ImportPlaidTransactionsTest, NegativeAmountMapsToEarnings) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData r{"ptx-4", -1200.0, "Paycheck", "2024-04-15", "", "USD"};
    PlaidSyncResult syncResult;
    syncResult.added = {r};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-4")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(curRepo, findByValue(_)).WillOnce(Return(std::nullopt));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    std::string capturedCatId;
    EXPECT_CALL(txRepo, create(_))
        .WillOnce([&capturedCatId](const Transaction& tx) {
            capturedCatId = tx.getCategoryId().getId();
            return tx;
        });

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    useCase.execute(makeCmd());

    EXPECT_EQ(capturedCatId, "earnings");
}

TEST(ImportPlaidTransactionsTest, ExecuteSetsNulloptCurrencyWhenCurrencyNotFound) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData r{"ptx-5", 20.0, "Coffee", "2024-04-05", "Food", "XYZ"};
    PlaidSyncResult syncResult;
    syncResult.added = {r};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-5")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(curRepo, findByValue("XYZ")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(txRepo, create(_)).WillOnce(Return(makeTx("tx-1", "user-1")));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    auto result = useCase.execute(makeCmd());

    ASSERT_EQ(result.size(), 1u);
}

TEST(ImportPlaidTransactionsTest, ExecuteReturnsEmptyWhenPlaidHasNoTransactions) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(PlaidSyncResult{}));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    auto result = useCase.execute(makeCmd());

    EXPECT_TRUE(result.empty());
}

TEST(ImportPlaidTransactionsTest, DuplicatePlaidTransactionIsSkipped) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData r{"ptx-dup", 10.0, "Coffee", "2024-04-01", "Food", "USD"};
    PlaidSyncResult syncResult;
    syncResult.added = {r};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    // Already exists — findByPlaidTransactionId returns a transaction.
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-dup"))
        .WillOnce(Return(makeTx("tx-existing", "user-1")));
    EXPECT_CALL(txRepo, create(_)).Times(0);
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    auto result = useCase.execute(makeCmd());

    EXPECT_TRUE(result.empty());
}

TEST(ImportPlaidTransactionsTest, ModifiedTransactionIsUpdated) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidTransactionData mod{"ptx-mod", 99.0, "Updated Desc", "2024-04-10", "Food", "USD"};
    PlaidSyncResult syncResult;
    syncResult.modified = {mod};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-mod"))
        .WillOnce(Return(makeTx("tx-existing", "user-1")));
    EXPECT_CALL(curRepo, findByValue("USD")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(txRepo, update(_)).WillOnce(Return(true));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    useCase.execute(makeCmd());
}

TEST(ImportPlaidTransactionsTest, RemovedTransactionIsDeleted) {
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;

    PlaidSyncResult syncResult;
    syncResult.removed = {"ptx-gone"};

    EXPECT_CALL(plaid, fetchTransactions(_, _)).WillOnce(Return(syncResult));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-gone"))
        .WillOnce(Return(makeTx("tx-existing", "user-1", "expenses")));
    EXPECT_CALL(txRepo, remove(TransactionId{"tx-existing"})).WillOnce(Return(true));
    EXPECT_CALL(plaidItemRepo, findById(_)).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions useCase(txRepo, curRepo, plaid, plaidItemRepo);
    useCase.execute(makeCmd());
}
