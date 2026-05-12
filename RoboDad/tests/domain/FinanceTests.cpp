#include <gtest/gtest.h>
#include "finance/Finance.hpp"
#include "finance/FinanceEnum.hpp"

TEST(FinanceTest, FullConstructorSetsAllFields) {
    Finance f(1u, 5000.0, "Monthly salary", FinanceEnum::Salary);
    EXPECT_EQ(f.getId(), 1u);
    EXPECT_DOUBLE_EQ(f.getAmount(), 5000.0);
    EXPECT_EQ(f.getDescription(), "Monthly salary");
    EXPECT_EQ(f.getType(), FinanceEnum::Salary);
}

TEST(FinanceTest, NewFinanceConstructorSetsIdToZero) {
    Finance f(3000.0, "Freelance", FinanceEnum::Bonus);
    EXPECT_EQ(f.getId(), 0u);
    EXPECT_EQ(f.getType(), FinanceEnum::Bonus);
}

TEST(FinanceTest, AllEnumTypesAreAccessible) {
    EXPECT_EQ(Finance(0.0, "s", FinanceEnum::Salary).getType(),     FinanceEnum::Salary);
    EXPECT_EQ(Finance(0.0, "b", FinanceEnum::Bonus).getType(),      FinanceEnum::Bonus);
    EXPECT_EQ(Finance(0.0, "i", FinanceEnum::Investment).getType(), FinanceEnum::Investment);
    EXPECT_EQ(Finance(0.0, "e", FinanceEnum::Expense).getType(),    FinanceEnum::Expense);
    EXPECT_EQ(Finance(0.0, "t", FinanceEnum::Tax).getType(),        FinanceEnum::Tax);
}

TEST(FinanceTest, SettersUpdateAllFields) {
    Finance f(100.0, "initial", FinanceEnum::Salary);
    f.setId(5u);
    f.setAmount(999.99);
    f.setDescription("updated");
    f.setType(FinanceEnum::Tax);
    EXPECT_EQ(f.getId(), 5u);
    EXPECT_DOUBLE_EQ(f.getAmount(), 999.99);
    EXPECT_EQ(f.getDescription(), "updated");
    EXPECT_EQ(f.getType(), FinanceEnum::Tax);
}
