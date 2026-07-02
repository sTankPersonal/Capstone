#include <gtest/gtest.h>
#include <cstdlib>
#include "infrastructure/apiClient/plaidClient/plaidClient.h"

class PlaidClientTest : public ::testing::Test {
protected:
    std::string clientId_;
    std::string secret_;

    void SetUp() override {
        const char* id  = std::getenv("PLAID_CLIENT_ID");
        const char* sec = std::getenv("PLAID_SECRET");
        if (!id || !sec)
            GTEST_SKIP() << "PLAID_CLIENT_ID and PLAID_SECRET not set; skipping Plaid integration tests.";
        clientId_ = id;
        secret_   = sec;
    }
};

TEST_F(PlaidClientTest, CreateSandboxAccessTokenReturnsNonEmptyToken) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    EXPECT_FALSE(token.empty());
}

TEST_F(PlaidClientTest, FetchTransactionsReturnsNonEmptyList) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    auto syncResult = client.fetchTransactions(token);
    EXPECT_FALSE(syncResult.added.empty());
}

TEST_F(PlaidClientTest, FetchTransactionsHaveValidFields) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    auto syncResult = client.fetchTransactions(token);
    ASSERT_FALSE(syncResult.added.empty());
    for (const auto& tx : syncResult.added) {
        EXPECT_FALSE(tx.date.empty());
        EXPECT_FALSE(tx.description.empty());
        EXPECT_FALSE(tx.currencyCode.empty());
    }
}

TEST_F(PlaidClientTest, FetchTransactionsAcceptsCursorParameter) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    auto firstSync = client.fetchTransactions(token);
    EXPECT_NO_THROW({
        auto secondSync = client.fetchTransactions(token, firstSync.nextCursor);
        (void)secondSync;
    });
}
