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
    auto txns = client.fetchTransactions(token, "2024-01-01", "2024-03-31", 10);
    EXPECT_FALSE(txns.empty());
}

TEST_F(PlaidClientTest, FetchTransactionsHaveValidFields) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    auto txns = client.fetchTransactions(token, "2024-01-01", "2024-03-31", 5);
    ASSERT_FALSE(txns.empty());
    for (const auto& tx : txns) {
        EXPECT_FALSE(tx.date.empty());
        EXPECT_FALSE(tx.description.empty());
        EXPECT_FALSE(tx.currencyCode.empty());
    }
}

TEST_F(PlaidClientTest, FetchTransactionsRespectsCountLimit) {
    PlaidClient client(clientId_, secret_);
    const std::string token = client.createSandboxAccessToken();
    const int limit = 3;
    auto txns = client.fetchTransactions(token, "2024-01-01", "2024-12-31", limit);
    EXPECT_LE(static_cast<int>(txns.size()), limit);
}
