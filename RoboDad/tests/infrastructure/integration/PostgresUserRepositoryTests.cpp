#include <gtest/gtest.h>
#include <algorithm>
#include <cstdlib>
#include <memory>
#include <vector>
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "domain/user/EmploymentStatus.h"
#include "domain/user/PersonalInfo.h"
#include "domain/user/User.h"

class PostgresUserRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* url = std::getenv("DATABASE_URL");
        ASSERT_NE(url, nullptr) << "DATABASE_URL must be set for integration tests";
        db_       = std::make_unique<DatabaseConnection>(url);
        userRepo_ = std::make_unique<PostgresUserRepository>(*db_);
    }

    void TearDown() override {
        for (uint32_t id : createdIds_)
            userRepo_->remove(id);
    }

    User createUser(const std::string& email,
                    const std::string& name   = "TestUser",
                    uint8_t            age    = 30,
                    EmploymentStatus   status = EmploymentStatus::Employed,
                    const std::string& hash   = "placeholder_hash") {
        PersonalInfo info(name, age, status);
        User user = userRepo_->create(User(0, info, {}), email, hash);
        createdIds_.push_back(user.getId());
        return user;
    }

    std::unique_ptr<DatabaseConnection>     db_;
    std::unique_ptr<PostgresUserRepository> userRepo_;
    std::vector<uint32_t>                   createdIds_;
};

TEST_F(PostgresUserRepositoryTest, CreateAssignsNewId) {
    User created = createUser("usertest_create@example.com");
    EXPECT_GT(created.getId(), 0u);
}

TEST_F(PostgresUserRepositoryTest, CreateStoresCorrectPersonalInfo) {
    User created = createUser("usertest_info@example.com", "Alice", 28, EmploymentStatus::Student);
    EXPECT_EQ(created.getPersonalInfo().getName(), "Alice");
    EXPECT_EQ(created.getPersonalInfo().getAge(), 28);
    EXPECT_EQ(created.getPersonalInfo().getEmploymentStatus(), EmploymentStatus::Student);
}

TEST_F(PostgresUserRepositoryTest, FindByIdReturnsCreatedUser) {
    User created = createUser("usertest_findbyid@example.com", "Bob", 25);
    auto found = userRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId(), created.getId());
    EXPECT_EQ(found->getPersonalInfo().getName(), "Bob");
    EXPECT_EQ(found->getPersonalInfo().getAge(), 25);
}

TEST_F(PostgresUserRepositoryTest, FindByIdReturnsNulloptForMissingUser) {
    EXPECT_FALSE(userRepo_->findById(999999u).has_value());
}

TEST_F(PostgresUserRepositoryTest, FindAllIncludesCreatedUser) {
    User created = createUser("usertest_findall@example.com");
    auto all = userRepo_->findAll();
    bool found = std::any_of(all.begin(), all.end(),
        [&](const User& u) { return u.getId() == created.getId(); });
    EXPECT_TRUE(found);
}

TEST_F(PostgresUserRepositoryTest, UpdateModifiesPersonalInfo) {
    User created = createUser("usertest_update@example.com");

    PersonalInfo updated("UpdatedName", 40, EmploymentStatus::Retired);
    EXPECT_TRUE(userRepo_->update(User(created.getId(), updated, {})));

    auto found = userRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getPersonalInfo().getName(), "UpdatedName");
    EXPECT_EQ(found->getPersonalInfo().getAge(), 40);
    EXPECT_EQ(found->getPersonalInfo().getEmploymentStatus(), EmploymentStatus::Retired);
}

TEST_F(PostgresUserRepositoryTest, UpdateReturnsFalseForMissingUser) {
    PersonalInfo info("Ghost", 20, EmploymentStatus::Unemployed);
    EXPECT_FALSE(userRepo_->update(User(999999u, info, {})));
}

TEST_F(PostgresUserRepositoryTest, RemoveDeletesUser) {
    PersonalInfo info("ToDelete", 22, EmploymentStatus::Employed);
    User created = userRepo_->create(User(0, info, {}), "usertest_remove@example.com", "hash");
    EXPECT_TRUE(userRepo_->remove(created.getId()));
    EXPECT_FALSE(userRepo_->findById(created.getId()).has_value());
}

TEST_F(PostgresUserRepositoryTest, RemoveReturnsFalseForMissingUser) {
    EXPECT_FALSE(userRepo_->remove(999999u));
}

TEST_F(PostgresUserRepositoryTest, LookupCredentialsReturnsIdAndHashForKnownEmail) {
    const std::string storedHash = "test_stored_hash_abc123";
    User created = createUser("usertest_lookup@example.com", "TestUser", 30,
                              EmploymentStatus::Employed, storedHash);

    auto creds = userRepo_->lookupCredentials("usertest_lookup@example.com");
    ASSERT_TRUE(creds.has_value());
    EXPECT_EQ(creds->first, created.getId());
    EXPECT_EQ(creds->second, storedHash);
}

TEST_F(PostgresUserRepositoryTest, LookupCredentialsReturnsNulloptForUnknownEmail) {
    EXPECT_FALSE(userRepo_->lookupCredentials("no_such_user@example.com").has_value());
}
