#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "IPlaidClient.h"
#include "IPlaidItemRepository.h"
#include "ITransactionRepository.h"
#include "ICurrencyRepository.h"

#include "CreatePlaidLinkToken.h"
#include "CreatePlaidLinkTokenCommand.h"
#include "LinkPlaidAccount.h"
#include "LinkPlaidAccountCommand.h"
#include "ImportPlaidTransactions.h"

#include "PlaidItem.h"
#include "PlaidItemId.h"
#include "PlaidTransactionData.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "TransactionCategoryId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "Currency.h"
#include "CurrencyId.h"
#include "UserId.h"

#include <chrono>

using ::testing::_;
using ::testing::Return;

namespace {

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)},
            std::chrono::day{static_cast<unsigned>(d)}};
}

static Transaction makeTx(const std::string& id, const std::string& userId) {
    auto d = makeDate(2024, 1, 1);
    return Transaction(
        TransactionId{id}, UserId{userId}, TransactionCategoryId{"expenses"},
        TransactionAmount(std::optional<double>{25.0}, std::optional<CurrencyId>{}),
        TransactionDescription(std::optional<std::string>{"Test tx"}),
        d, d
    );
}

// ── Mocks ─────────────────────────────────────────────────────────────────────

class MockPlaidClient : public IPlaidClient {
public:
    MOCK_METHOD(std::string, createLinkToken,         (const std::string&),                                      (override));
    MOCK_METHOD(std::string, exchangePublicToken,      (const std::string&),                                      (override));
    MOCK_METHOD(std::string, createSandboxAccessToken, (const std::string&),                                      (override));
    MOCK_METHOD(PlaidSyncResult, fetchTransactions,
                (const std::string&, const std::string&), (override));
};

class MockPlaidItemRepository : public IPlaidItemRepository {
public:
    MOCK_METHOD(PlaidItem,                create,       (const PlaidItem&),  (override));
    MOCK_METHOD(std::optional<PlaidItem>, findById,     (PlaidItemId),       (override));
    MOCK_METHOD(std::vector<PlaidItem>,   findAll,      (),                  (override));
    MOCK_METHOD(bool,                     update,       (const PlaidItem&),  (override));
    MOCK_METHOD(bool,                     remove,       (PlaidItemId),       (override));
    MOCK_METHOD(std::optional<PlaidItem>, findByUserId, (const UserId&),     (override));
};

class MockTransactionRepository : public ITransactionRepository {
public:
    MOCK_METHOD(Transaction,                create,       (const Transaction&), (override));
    MOCK_METHOD(std::optional<Transaction>, findById,     (TransactionId),      (override));
    MOCK_METHOD(std::vector<Transaction>,   findAll,      (),                   (override));
    MOCK_METHOD(bool,                       update,       (const Transaction&), (override));
    MOCK_METHOD(bool,                       remove,       (TransactionId),      (override));
    MOCK_METHOD(std::vector<Transaction>,   findByUserId, (const UserId&),      (override));
    MOCK_METHOD(std::optional<Transaction>, findByPlaidTransactionId, (const std::string&), (override));
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

} // namespace

// ── CreatePlaidLinkToken ──────────────────────────────────────────────────────

TEST(CreatePlaidLinkTokenTest, ReturnsLinkTokenFromClient) {
    MockPlaidClient plaid;
    EXPECT_CALL(plaid, createLinkToken("user-42"))
        .WillOnce(Return("link-sandbox-abc123"));

    CreatePlaidLinkToken useCase(plaid);
    std::string token = useCase.execute(CreatePlaidLinkTokenCommand{UserId{"user-42"}});

    EXPECT_EQ(token, "link-sandbox-abc123");
}

TEST(CreatePlaidLinkTokenTest, PassesUserIdToClient) {
    MockPlaidClient plaid;
    EXPECT_CALL(plaid, createLinkToken("user-99"))
        .WillOnce(Return("link-sandbox-xyz"));

    CreatePlaidLinkToken useCase(plaid);
    useCase.execute(CreatePlaidLinkTokenCommand{UserId{"user-99"}});
}

// ── LinkPlaidAccount ──────────────────────────────────────────────────────────

TEST(LinkPlaidAccountTest, ExchangesPublicTokenAndStoresItem) {
    MockPlaidClient         plaid;
    MockPlaidItemRepository plaidItemRepo;
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;

    EXPECT_CALL(plaid, exchangePublicToken("public-tok-123"))
        .WillOnce(Return("access-sandbox-abc"));

    EXPECT_CALL(plaidItemRepo, create(_))
        .WillOnce([](const PlaidItem& item) { return item; });

    EXPECT_CALL(plaid, fetchTransactions("access-sandbox-abc", ""))
        .WillOnce(Return(PlaidSyncResult{}));

    ImportPlaidTransactions importUseCase(txRepo, curRepo, plaid, plaidItemRepo);
    LinkPlaidAccount useCase(plaid, plaidItemRepo, importUseCase);

    auto result = useCase.execute(
        LinkPlaidAccountCommand{UserId{"user-1"}, "public-tok-123"});

    EXPECT_TRUE(result.empty());
}

TEST(LinkPlaidAccountTest, ImportsTransactionsAfterExchange) {
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;

    EXPECT_CALL(plaid, exchangePublicToken("public-tok-456"))
        .WillOnce(Return("access-sandbox-def"));

    EXPECT_CALL(plaidItemRepo, create(_))
        .WillOnce([](const PlaidItem& item) { return item; });

    PlaidTransactionData r1{"ptx-1", 12.0, "Coffee",    "2024-02-10", "Food", "USD"};
    PlaidTransactionData r2{"ptx-2", 50.0, "Groceries", "2024-02-11", "Food", "USD"};
    PlaidSyncResult syncResult;
    syncResult.added = {r1, r2};
    EXPECT_CALL(plaid, fetchTransactions("access-sandbox-def", ""))
        .WillOnce(Return(syncResult));

    EXPECT_CALL(curRepo, findByValue("USD"))
        .Times(2)
        .WillRepeatedly(Return(std::optional<Currency>{
            Currency(CurrencyId{"USD"}, "USD", makeDate(2024,1,1))}));

    EXPECT_CALL(txRepo, create(_))
        .WillOnce(Return(makeTx("tx-1", "user-1")))
        .WillOnce(Return(makeTx("tx-2", "user-1")));

    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-1")).WillOnce(Return(std::nullopt));
    EXPECT_CALL(txRepo, findByPlaidTransactionId("ptx-2")).WillOnce(Return(std::nullopt));

    ImportPlaidTransactions importUseCase(txRepo, curRepo, plaid, plaidItemRepo);
    LinkPlaidAccount useCase(plaid, plaidItemRepo, importUseCase);

    auto result = useCase.execute(
        LinkPlaidAccountCommand{UserId{"user-1"}, "public-tok-456"});

    ASSERT_EQ(result.size(), 2u);
}

TEST(LinkPlaidAccountTest, StoredAccessTokenMatchesExchangedToken) {
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;

    EXPECT_CALL(plaid, exchangePublicToken("public-tok-789"))
        .WillOnce(Return("access-sandbox-ghi"));

    std::string capturedToken;
    EXPECT_CALL(plaidItemRepo, create(_))
        .WillOnce([&capturedToken](const PlaidItem& item) {
            capturedToken = item.getAccessToken();
            return item;
        });

    EXPECT_CALL(plaid, fetchTransactions("access-sandbox-ghi", _))
        .WillOnce(Return(PlaidSyncResult{}));

    ImportPlaidTransactions importUseCase(txRepo, curRepo, plaid, plaidItemRepo);
    LinkPlaidAccount useCase(plaid, plaidItemRepo, importUseCase);

    useCase.execute(
        LinkPlaidAccountCommand{UserId{"user-1"}, "public-tok-789"});

    EXPECT_EQ(capturedToken, "access-sandbox-ghi");
}

TEST(LinkPlaidAccountTest, ImportsWithInitialEmptyCursor) {
    MockPlaidClient           plaid;
    MockPlaidItemRepository   plaidItemRepo;
    MockTransactionRepository txRepo;
    MockCurrencyRepository    curRepo;

    EXPECT_CALL(plaid, exchangePublicToken(_)).WillOnce(Return("access-sandbox-xyz"));
    EXPECT_CALL(plaidItemRepo, create(_)).WillOnce([](const PlaidItem& item) { return item; });

    EXPECT_CALL(plaid, fetchTransactions(_, ""))
        .WillOnce(Return(PlaidSyncResult{}));

    ImportPlaidTransactions importUseCase(txRepo, curRepo, plaid, plaidItemRepo);
    LinkPlaidAccount useCase(plaid, plaidItemRepo, importUseCase);

    useCase.execute(
        LinkPlaidAccountCommand{UserId{"user-1"}, "pub-tok"});
}
