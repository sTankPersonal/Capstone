#include <gtest/gtest.h>
#include "user/User.hpp"
#include "user/PersonalInfo.hpp"
#include "user/EmploymentStatus.hpp"
#include "finance/Finance.hpp"
#include "finance/FinanceEnum.hpp"

TEST(UserTest, ConstructorPreservesIdAndPersonalInfo) {
    PersonalInfo info("Alice", 30, EmploymentStatus::Employed);
    User user(1u, info, {});
    EXPECT_EQ(user.getId(), 1u);
    EXPECT_EQ(user.getPersonalInfo().getName(), "Alice");
    EXPECT_EQ(user.getPersonalInfo().getAge(), 30u);
    EXPECT_EQ(user.getPersonalInfo().getEmploymentStatus(), EmploymentStatus::Employed);
}

TEST(UserTest, UserWithNoFinancesHasEmptyVector) {
    PersonalInfo info("Bob", 25, EmploymentStatus::Student);
    User user(2u, info, {});
    EXPECT_TRUE(user.getFinances().empty());
}

TEST(UserTest, UserStoresMultipleFinanceEntries) {
    PersonalInfo info("Carol", 40, EmploymentStatus::Employed);
    std::vector<Finance> finances = {
        Finance(1u, 4000.0, "Salary",      FinanceEnum::Salary),
        Finance(2u,  200.0, "Coffee shop", FinanceEnum::Expense),
    };
    User user(3u, info, finances);
    ASSERT_EQ(user.getFinances().size(), 2u);
    EXPECT_DOUBLE_EQ(user.getFinances()[0].getAmount(), 4000.0);
    EXPECT_EQ(user.getFinances()[1].getType(), FinanceEnum::Expense);
}

TEST(UserTest, SetPersonalInfoReplacesExistingInfo) {
    PersonalInfo original("Dave", 22, EmploymentStatus::Unemployed);
    User user(4u, original, {});
    PersonalInfo updated("Dave Jr.", 23, EmploymentStatus::Employed);
    user.setPersonalInfo(updated);
    EXPECT_EQ(user.getPersonalInfo().getName(), "Dave Jr.");
    EXPECT_EQ(user.getPersonalInfo().getEmploymentStatus(), EmploymentStatus::Employed);
}

TEST(UserTest, SetFinancesReplacesExistingFinances) {
    PersonalInfo info("Eve", 35, EmploymentStatus::Employed);
    User user(5u, info, {Finance(1u, 100.0, "old", FinanceEnum::Expense)});
    user.setFinances({});
    EXPECT_TRUE(user.getFinances().empty());
}
