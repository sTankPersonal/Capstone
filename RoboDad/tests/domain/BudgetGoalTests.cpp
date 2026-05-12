#include <gtest/gtest.h>
#include "budget/BudgetGoal.hpp"

TEST(BudgetGoalTest, NewGoalConstructorSetsIdAndUserIdToZero) {
    BudgetGoal goal(1000.0, 250.0, "Emergency fund");
    EXPECT_EQ(goal.getId(), 0u);
    EXPECT_EQ(goal.getUserId(), 0u);
}

TEST(BudgetGoalTest, NewGoalConstructorPreservesValues) {
    BudgetGoal goal(1000.0, 250.0, "Emergency fund");
    EXPECT_DOUBLE_EQ(goal.getEndGoal(), 1000.0);
    EXPECT_DOUBLE_EQ(goal.getAmount(), 250.0);
    EXPECT_EQ(goal.getDescription(), "Emergency fund");
}

TEST(BudgetGoalTest, FullConstructorSetsAllFields) {
    BudgetGoal goal(42u, 7u, 5000.0, 1200.0, "Vacation");
    EXPECT_EQ(goal.getId(), 42u);
    EXPECT_EQ(goal.getUserId(), 7u);
    EXPECT_DOUBLE_EQ(goal.getEndGoal(), 5000.0);
    EXPECT_DOUBLE_EQ(goal.getAmount(), 1200.0);
    EXPECT_EQ(goal.getDescription(), "Vacation");
}

TEST(BudgetGoalTest, SettersUpdateAllFields) {
    BudgetGoal goal(100.0, 50.0, "Start");
    goal.setId(99u);
    goal.setUserId(3u);
    goal.setEndGoal(200.0);
    goal.setAmount(75.0);
    goal.setDescription("Updated");
    EXPECT_EQ(goal.getId(), 99u);
    EXPECT_EQ(goal.getUserId(), 3u);
    EXPECT_DOUBLE_EQ(goal.getEndGoal(), 200.0);
    EXPECT_DOUBLE_EQ(goal.getAmount(), 75.0);
    EXPECT_EQ(goal.getDescription(), "Updated");
}

TEST(BudgetGoalTest, AmountLessThanEndGoalRepresentsInProgress) {
    BudgetGoal goal(1000.0, 300.0, "New car");
    EXPECT_LT(goal.getAmount(), goal.getEndGoal());
}
