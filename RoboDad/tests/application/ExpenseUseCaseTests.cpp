#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "infrastructureServices/persistance/ExpenseRepository.h"
#include "application/expenses/AddExpense.h"
#include "application/expenses/ListExpenses.h"
#include "domain/finance/Finance.h"
#include "domain/finance/FinanceEnum.h"

using ::testing::Return;
using ::testing::_;

class MockExpenseRepository : public ExpenseRepository {
public:
    MOCK_METHOD(Finance,                   create,       (uint32_t, const Finance&), (override));
    MOCK_METHOD(std::optional<Finance>,    findById,     (uint32_t),                 (override));
    MOCK_METHOD(std::vector<Finance>,      findByUserId, (uint32_t),                 (override));
    MOCK_METHOD(bool,                      update,       (const Finance&),            (override));
    MOCK_METHOD(bool,                      remove,       (uint32_t),                 (override));
};

TEST(AddExpenseTest, ExecuteCreatesExpenseForCorrectUser) {
    MockExpenseRepository repo;
    Finance expected(1u, 49.99, "Grocery run", FinanceEnum::Expense);
    EXPECT_CALL(repo, create(3u, _)).WillOnce(Return(expected));

    AddExpense useCase(repo);
    Finance result = useCase.execute(3u, 49.99, "Grocery run", FinanceEnum::Expense);

    EXPECT_EQ(result.getId(), 1u);
    EXPECT_DOUBLE_EQ(result.getAmount(), 49.99);
    EXPECT_EQ(result.getType(), FinanceEnum::Expense);
}

TEST(ListExpensesTest, ExecuteReturnsAllExpensesForUser) {
    MockExpenseRepository repo;
    std::vector<Finance> expenses = {
        Finance(1u, 49.99, "Groceries", FinanceEnum::Expense),
        Finance(2u, 12.50, "Bus pass",  FinanceEnum::Expense),
    };
    EXPECT_CALL(repo, findByUserId(3u)).WillOnce(Return(expenses));

    ListExpenses useCase(repo);
    auto result = useCase.execute(3u);

    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].getDescription(), "Groceries");
    EXPECT_EQ(result[1].getDescription(), "Bus pass");
}

TEST(ListExpensesTest, ExecuteReturnsEmptyVectorForNewUser) {
    MockExpenseRepository repo;
    EXPECT_CALL(repo, findByUserId(99u)).WillOnce(Return(std::vector<Finance>{}));

    ListExpenses useCase(repo);
    EXPECT_TRUE(useCase.execute(99u).empty());
}
