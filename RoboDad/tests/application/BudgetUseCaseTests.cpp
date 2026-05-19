#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "infrastructureServices/persistance/BudgetRepository.h"
#include "application/budgets/GetBudgetStatus.h"
#include "application/budgets/SetBudgetGoal.h"
#include "budget/BudgetGoal.hpp"

using ::testing::Return;
using ::testing::_;

class MockBudgetRepository : public BudgetRepository {
public:
    MOCK_METHOD(BudgetGoal,                  create,        (const BudgetGoal&), (override));
    MOCK_METHOD(std::optional<BudgetGoal>,   findById,      (uint32_t),          (override));
    MOCK_METHOD(std::vector<BudgetGoal>,     findByUserId,  (uint32_t),          (override));
    MOCK_METHOD(bool,                        update,        (const BudgetGoal&), (override));
    MOCK_METHOD(bool,                        remove,        (uint32_t),          (override));
};

TEST(SetBudgetGoalTest, ExecuteCreatesGoalWithCorrectValues) {
    MockBudgetRepository repo;
    BudgetGoal expected(1u, 7u, 500.0, 100.0, "Vacation savings");
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    SetBudgetGoal useCase(repo);
    BudgetGoal result = useCase.execute(7u, 500.0, 100.0, "Vacation savings");

    EXPECT_EQ(result.getUserId(), 7u);
    EXPECT_DOUBLE_EQ(result.getEndGoal(), 500.0);
    EXPECT_DOUBLE_EQ(result.getAmount(), 100.0);
    EXPECT_EQ(result.getDescription(), "Vacation savings");
}

TEST(GetBudgetStatusTest, ExecuteReturnsGoalsForUser) {
    MockBudgetRepository repo;
    std::vector<BudgetGoal> goals = {
        BudgetGoal(1u, 7u, 500.0, 100.0, "Goal A"),
        BudgetGoal(2u, 7u, 200.0,  50.0, "Goal B"),
    };
    EXPECT_CALL(repo, findByUserId(7u)).WillOnce(Return(goals));

    GetBudgetStatus useCase(repo);
    auto result = useCase.execute(7u);

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getDescription(), "Goal A");
    EXPECT_EQ(result[1].getDescription(), "Goal B");
}

TEST(GetBudgetStatusTest, ExecuteReturnsEmptyVectorForUserWithNoGoals) {
    MockBudgetRepository repo;
    EXPECT_CALL(repo, findByUserId(99u)).WillOnce(Return(std::vector<BudgetGoal>{}));

    GetBudgetStatus useCase(repo);
    EXPECT_TRUE(useCase.execute(99u).empty());
}
