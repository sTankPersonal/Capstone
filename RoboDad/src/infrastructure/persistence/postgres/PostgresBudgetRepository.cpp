#include "infrastructure/persistence/postgres/PostgresBudgetRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>

namespace {

BudgetGoal rowToBudgetGoal(PGresult* res, int row) {
    uint32_t id     = static_cast<uint32_t>(std::stoul(PQgetvalue(res, row, 0)));
    uint32_t userId = static_cast<uint32_t>(std::stoul(PQgetvalue(res, row, 1)));
    double endGoal  = std::stod(PQgetvalue(res, row, 2));
    double amount   = std::stod(PQgetvalue(res, row, 3));
    std::string desc = PQgetvalue(res, row, 4);
    return BudgetGoal(id, userId, endGoal, amount, desc);
}

} // namespace

PostgresBudgetRepository::PostgresBudgetRepository(DatabaseConnection& db) : db_(db) {}

BudgetGoal PostgresBudgetRepository::create(const BudgetGoal& goal) {
    std::string uid     = std::to_string(goal.getUserId());
    std::string endGoal = std::to_string(goal.getEndGoal());
    std::string amount  = std::to_string(goal.getAmount());
    std::string desc    = goal.getDescription();

    const char* p[] = { uid.c_str(), endGoal.c_str(), amount.c_str(), desc.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO budget_goals (user_id, end_goal, amount, description)"
        " VALUES ($1::integer, $2::double precision, $3::double precision, $4) RETURNING id",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "BudgetGoal::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    BudgetGoal created = goal;
    created.setId(newId);
    return created;
}

std::optional<BudgetGoal> PostgresBudgetRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, user_id, end_goal, amount, description FROM budget_goals WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "BudgetGoal::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;
    return rowToBudgetGoal(res.get(), 0);
}

std::vector<BudgetGoal> PostgresBudgetRepository::findByUserId(uint32_t userId) {
    std::string uid = std::to_string(userId);
    const char* p[] = { uid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, user_id, end_goal, amount, description FROM budget_goals WHERE user_id = $1 ORDER BY id",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "BudgetGoal::findByUserId");

    std::vector<BudgetGoal> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) out.push_back(rowToBudgetGoal(res.get(), i));
    return out;
}

bool PostgresBudgetRepository::update(const BudgetGoal& goal) {
    std::string sid     = std::to_string(goal.getId());
    std::string endGoal = std::to_string(goal.getEndGoal());
    std::string amount  = std::to_string(goal.getAmount());
    std::string desc    = goal.getDescription();

    const char* p[] = { endGoal.c_str(), amount.c_str(), desc.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE budget_goals SET end_goal=$1::double precision, amount=$2::double precision, description=$3 WHERE id=$4",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "BudgetGoal::update");
    return pg::rowsAffected(res.get());
}

bool PostgresBudgetRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM budget_goals WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "BudgetGoal::remove");
    return pg::rowsAffected(res.get());
}
