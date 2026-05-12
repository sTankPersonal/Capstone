#include "application/expenses/ListExpenses.h"

std::vector<Finance> ListExpenses::execute(uint32_t userId) {
    return repo_.findByUserId(userId);
}
