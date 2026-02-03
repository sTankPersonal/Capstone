#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "User.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include <chrono>

static std::chrono::year_month_day today() {
    return std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

class PostgresUserRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* url = std::getenv("DATABASE_URL");
        ASSERT_NE(url, nullptr) << "DATABASE_URL must be set for integration tests";
        db_       = std::make_unique<DatabaseConnection>(url);
        userRepo_ = std::make_unique<PostgresUserRepository>(*db_);
    }

    void TearDown() override {
        for (const UserId& id : createdIds_)
            userRepo_->remove(id);
    }

    User createUser(const std::string& email,
                    const std::string& passwordHash = "placeholder_hash",
                    const std::string& firstName    = "TestUser") {
        auto d = today();
        User newUser(
            UserId{"test-" + email},
            UserLogin(email, std::optional<std::string>{passwordHash}),
            UserInformation(firstName, std::nullopt, std::nullopt,
                            std::nullopt, std::nullopt, std::nullopt, std::nullopt),
            d, d
        );
        User created = userRepo_->create(newUser);
        createdIds_.push_back(created.getId());
        return created;
    }

    std::unique_ptr<DatabaseConnection>     db_;
    std::unique_ptr<PostgresUserRepository> userRepo_;
    std::vector<UserId>                     createdIds_;
};

TEST_F(PostgresUserRepositoryTest, CreateReturnsUserWithId) {
    User created = createUser("usertest_create@example.com");
    EXPECT_FALSE(created.getId().getId().empty());
}

TEST_F(PostgresUserRepositoryTest, CreateStoresCorrectEmail) {
    User created = createUser("usertest_email@example.com");
    EXPECT_EQ(created.getUserLogin().getEmail(), "usertest_email@example.com");
}

TEST_F(PostgresUserRepositoryTest, CreateStoresFirstName) {
    User created = createUser("usertest_info@example.com", "hash", "Alice");
    ASSERT_TRUE(created.getUserInformation().getFirstName().has_value());
    EXPECT_EQ(created.getUserInformation().getFirstName().value(), "Alice");
}

TEST_F(PostgresUserRepositoryTest, FindByIdReturnsCreatedUser) {
    User created = createUser("usertest_findbyid@example.com", "hash", "Bob");
    auto found = userRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId().getId(), created.getId().getId());
    ASSERT_TRUE(found->getUserInformation().getFirstName().has_value());
    EXPECT_EQ(found->getUserInformation().getFirstName().value(), "Bob");
}

TEST_F(PostgresUserRepositoryTest, FindByIdReturnsNulloptForMissingUser) {
    EXPECT_FALSE(userRepo_->findById(UserId{"non-existent-uuid-99999"}).has_value());
}

TEST_F(PostgresUserRepositoryTest, FindAllIncludesCreatedUser) {
    User created = createUser("usertest_findall@example.com");
    auto all = userRepo_->findAll();
    bool found = std::any_of(all.begin(), all.end(),
        [&](const User& u) { return u.getId().getId() == created.getId().getId(); });
    EXPECT_TRUE(found);
}

TEST_F(PostgresUserRepositoryTest, UpdateModifiesUserInformation) {
    User created = createUser("usertest_update@example.com", "hash", "Original");
    auto d = today();
    User updated(
        created.getId(),
        UserLogin("usertest_update@example.com", std::nullopt),
        UserInformation("Updated", std::nullopt, std::nullopt,
                        std::nullopt, std::nullopt, std::nullopt, std::nullopt),
        d, d
    );
    EXPECT_TRUE(userRepo_->update(updated));

    auto found = userRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    ASSERT_TRUE(found->getUserInformation().getFirstName().has_value());
    EXPECT_EQ(found->getUserInformation().getFirstName().value(), "Updated");
}

TEST_F(PostgresUserRepositoryTest, UpdateReturnsFalseForMissingUser) {
    auto d = today();
    User ghost(
        UserId{"non-existent-uuid-ghost"},
        UserLogin("ghost@example.com", std::nullopt),
        UserInformation{},
        d, d
    );
    EXPECT_FALSE(userRepo_->update(ghost));
}

TEST_F(PostgresUserRepositoryTest, RemoveDeletesUser) {
    auto d = today();
    User newUser(
        UserId{"test-usertest_remove@example.com"},
        UserLogin("usertest_remove@example.com", std::optional<std::string>{"hash"}),
        UserInformation{},
        d, d
    );
    User created = userRepo_->create(newUser);
    EXPECT_TRUE(userRepo_->remove(created.getId()));
    EXPECT_FALSE(userRepo_->findById(created.getId()).has_value());
}

TEST_F(PostgresUserRepositoryTest, RemoveReturnsFalseForMissingUser) {
    EXPECT_FALSE(userRepo_->remove(UserId{"non-existent-uuid-99999"}));
}

TEST_F(PostgresUserRepositoryTest, LookupCredentialsReturnsIdAndHashForKnownEmail) {
    const std::string storedHash = "test_stored_hash_abc123";
    User created = createUser("usertest_lookup@example.com", storedHash);

    auto creds = userRepo_->lookupCredentials("usertest_lookup@example.com");
    ASSERT_TRUE(creds.has_value());
    EXPECT_EQ(creds->first.getId(), created.getId().getId());
    EXPECT_EQ(creds->second, storedHash);
}

TEST_F(PostgresUserRepositoryTest, LookupCredentialsReturnsNulloptForUnknownEmail) {
    EXPECT_FALSE(userRepo_->lookupCredentials("no_such_user@example.com").has_value());
}
