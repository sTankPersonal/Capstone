#include <gtest/gtest.h>
#include <cstdlib>
#include <memory>
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresBudgetRepository.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "domain/budget/BudgetGoal.h"
#include "domain/user/EmploymentStatus.h"
#include "domain/user/PersonalInfo.h"
#include "domain/user/User.h"

class PostgresBudgetRepositoryTest : public ::testing::Test {
protected:
    void SetUp() override {
        const char* url = std::getenv("DATABASE_URL");
        ASSERT_NE(url, nullptr) << "DATABASE_URL must be set for integration tests";
        db_         = std::make_unique<DatabaseConnection>(url);
        userRepo_   = std::make_unique<PostgresUserRepository>(*db_);
        budgetRepo_ = std::make_unique<PostgresBudgetRepository>(*db_);

        PersonalInfo info("TestUser", 25, EmploymentStatus::Employed);
        testUser_ = userRepo_->create(User(0, info, {}));
    }

    void TearDown() override {
        if (db_ && testUser_.getId() != 0)
            userRepo_->remove(testUser_.getId()); // CASCADE deletes budget_goals
    }

    std::unique_ptr<DatabaseConnection>       db_;
    std::unique_ptr<PostgresUserRepository>   userRepo_;
    std::unique_ptr<PostgresBudgetRepository> budgetRepo_;
    User testUser_{0, PersonalInfo("", 0, EmploymentStatus::Employed), {}};
};

TEST_F(PostgresBudgetRepositoryTest, CreateAssignsNewId) {
    BudgetGoal goal(0u, testUser_.getId(), 500.0, 100.0, "Emergency fund");
    BudgetGoal created = budgetRepo_->create(goal);
    EXPECT_GT(created.getId(), 0u);
    EXPECT_EQ(created.getUserId(), testUser_.getId());
    budgetRepo_->remove(created.getId());
}

TEST_F(PostgresBudgetRepositoryTest, FindByIdReturnsCreatedGoal) {
    BudgetGoal created = budgetRepo_->create(BudgetGoal(0u, testUser_.getId(), 500.0, 100.0, "Test goal"));
    auto found = budgetRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_EQ(found->getId(), created.getId());
    EXPECT_EQ(found->getDescription(), "Test goal");
    EXPECT_DOUBLE_EQ(found->getEndGoal(), 500.0);
    budgetRepo_->remove(created.getId());
}

TEST_F(PostgresBudgetRepositoryTest, FindByIdReturnsNulloptForMissingGoal) {
    EXPECT_FALSE(budgetRepo_->findById(999999u).has_value());
}

TEST_F(PostgresBudgetRepositoryTest, FindByUserIdReturnsAllGoalsForUser) {
    BudgetGoal a = budgetRepo_->create(BudgetGoal(0u, testUser_.getId(), 100.0, 10.0, "Goal A"));
    BudgetGoal b = budgetRepo_->create(BudgetGoal(0u, testUser_.getId(), 200.0, 20.0, "Goal B"));

    auto goals = budgetRepo_->findByUserId(testUser_.getId());
    EXPECT_GE(goals.size(), 2u);

    budgetRepo_->remove(a.getId());
    budgetRepo_->remove(b.getId());
}

TEST_F(PostgresBudgetRepositoryTest, UpdateModifiesExistingGoal) {
    BudgetGoal created = budgetRepo_->create(BudgetGoal(0u, testUser_.getId(), 100.0, 0.0, "Original"));
    created.setAmount(50.0);
    created.setDescription("Updated");
    EXPECT_TRUE(budgetRepo_->update(created));

    auto found = budgetRepo_->findById(created.getId());
    ASSERT_TRUE(found.has_value());
    EXPECT_DOUBLE_EQ(found->getAmount(), 50.0);
    EXPECT_EQ(found->getDescription(), "Updated");
    budgetRepo_->remove(created.getId());
}

TEST_F(PostgresBudgetRepositoryTest, RemoveDeletesGoal) {
    BudgetGoal created = budgetRepo_->create(BudgetGoal(0u, testUser_.getId(), 100.0, 0.0, "ToDelete"));
    EXPECT_TRUE(budgetRepo_->remove(created.getId()));
    EXPECT_FALSE(budgetRepo_->findById(created.getId()).has_value());
}
