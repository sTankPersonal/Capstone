#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include <vector>
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionCategoryRepository.h"
#include "User.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include "Transactions.h"
#include "TransactionId.h"
#include "TransactionAmount.h"
#include "TransactionDescription.h"
#include "TransactionCategoryId.h"
#include "CurrencyId.h"
#include <chrono>

static std::chrono::year_month_day today() {
    return std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

class PostgresTransactionRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* url = std::getenv("DATABASE_URL");
        ASSERT_NE(url, nullptr) << "DATABASE_URL must be set for integration tests";
        db_          = std::make_unique<DatabaseConnection>(url);
        userRepo_    = std::make_unique<PostgresUserRepository>(*db_);
        txRepo_      = std::make_unique<PostgresTransactionRepository>(*db_);
        categoryRepo_= std::make_unique<PostgresTransactionCategoryRepository>(*db_);

        // Create a test user for all transaction tests.
        auto d = today();
        User newUser(
            UserId{"txtest-user"},
            UserLogin("txtest@example.com", std::optional<std::string>{"placeholder_hash"}),
            UserInformation{},
            d, d
        );
        testUser_ = userRepo_->create(newUser);

        // Resolve or create a test category.
        auto existing = categoryRepo_->findByValue("Test Category");
        if (existing.has_value()) {
            testCategoryId_ = existing->getId();
        } else {
            TransactionCategory cat = categoryRepo_->create(
                TransactionCategory(TransactionCategoryId{"txtest-cat"}, "Test Category", d));
            testCategoryId_ = cat.getId();
            ownedCategoryId_ = testCategoryId_;
        }
    }

    void TearDown() override {
        for (const TransactionId& id : createdTxIds_)
            txRepo_->remove(id);
        userRepo_->remove(testUser_.getId());
        if (ownedCategoryId_.getId() != std::string{})
            categoryRepo_->remove(ownedCategoryId_);
    }

    Transaction createTransaction(double amount = 100.0, const std::string& desc = "Test") {
        auto d = today();
        Transaction tx(
            TransactionId{"txtest-" + desc},
            testUser_.getId(),
            testCategoryId_,
            TransactionAmount(std::optional<double>{amount}, CurrencyId{"USD"}),
            TransactionDescription(std::optional<std::string>{desc}),
            d, d
        );
        Transaction created = txRepo_->create(tx);
        createdTxIds_.push_back(created.getId());
        return created;
    }

    std::unique_ptr<DatabaseConnection>                    db_;
    std::unique_ptr<PostgresUserRepository>                userRepo_;
    std::unique_ptr<PostgresTransactionRepository>         txRepo_;
    std::unique_ptr<PostgresTransactionCategoryRepository> categoryRepo_;
    User                                                   testUser_{
        UserId{""}, UserLogin("", std::nullopt), UserInformation{}, today(), today()};
    TransactionCategoryId                                  testCategoryId_;
    TransactionCategoryId                                  ownedCategoryId_;
    std::vector<TransactionId>                             createdTxIds_;
};

TEST_F(PostgresTransactionRepositoryTest, CreateReturnsTransactionWithId) {
    Transaction created = createTransaction(49.99, "Grocery run");
    EXPECT_FALSE(created.getId().getId().empty());
}

TEST_F(PostgresTransactionRepositoryTest, CreateStoresAmountAndDescription) {
    Transaction created = createTransaction(99.0, "Office supplies");
    ASSERT_TRUE(created.getAmount().getAmount().has_value());
    EXPECT_DOUBLE_EQ(created.getAmount().getAmount().value(), 99.0);
    ASSERT_TRUE(created.getDescription().getDescription().has_value());
    EXPECT_EQ(created.getDescription().getDescription().value(), "Office supplies");
}

TEST_F(PostgresTransactionRepositoryTest, FindByIdReturnsCreatedTransaction) {
    Transaction created = createTransaction(150.0, "Rent");
    auto found = txRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId().getId(), created.getId().getId());
    ASSERT_TRUE(found->getDescription().getDescription().has_value());
    EXPECT_EQ(found->getDescription().getDescription().value(), "Rent");
}

TEST_F(PostgresTransactionRepositoryTest, FindByIdReturnsNulloptForMissingTransaction) {
    EXPECT_FALSE(txRepo_->findById(TransactionId{"non-existent-tx-uuid"}).has_value());
}

TEST_F(PostgresTransactionRepositoryTest, FindByUserIdReturnsTransactionsForUser) {
    Transaction a = createTransaction(50.0, "Coffee");
    Transaction b = createTransaction(200.0, "Electricity");

    auto results = txRepo_->findByUserId(testUser_.getId());
    EXPECT_GE(results.size(), 2u);
}

TEST_F(PostgresTransactionRepositoryTest, RemoveDeletesTransaction) {
    auto d = today();
    Transaction tx(
        TransactionId{"txtest-remove"},
        testUser_.getId(), testCategoryId_,
        TransactionAmount(std::optional<double>{10.0}, CurrencyId{"USD"}),
        TransactionDescription(std::optional<std::string>{"ToDelete"}),
        d, d
    );
    Transaction created = txRepo_->create(tx);
    EXPECT_TRUE(txRepo_->remove(created.getId()));
    EXPECT_FALSE(txRepo_->findById(created.getId()).has_value());
}

TEST_F(PostgresTransactionRepositoryTest, RemoveReturnsFalseForMissingTransaction) {
    EXPECT_FALSE(txRepo_->remove(TransactionId{"non-existent-tx-uuid"}));
}
