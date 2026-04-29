#include "infrastructure/persistence/postgres/PostgresExpenseRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include "finance/FinanceEnum.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>

namespace {

Finance rowToFinance(PGresult* res, int row) {
    uint32_t id    = static_cast<uint32_t>(std::stoul(PQgetvalue(res, row, 0)));
    double amount  = std::stod(PQgetvalue(res, row, 1));
    std::string desc = PQgetvalue(res, row, 2);
    auto type      = static_cast<FinanceEnum>(std::stoi(PQgetvalue(res, row, 3)));
    return Finance(id, amount, desc, type);
}

} // namespace

PostgresExpenseRepository::PostgresExpenseRepository(DatabaseConnection& db) : db_(db) {}

Finance PostgresExpenseRepository::create(uint32_t userId, const Finance& finance) {
    std::string uid    = std::to_string(userId);
    std::string amount = std::to_string(finance.getAmount());
    std::string desc   = finance.getDescription();
    std::string type   = std::to_string(static_cast<int>(finance.getType()));

    const char* p[] = { uid.c_str(), amount.c_str(), desc.c_str(), type.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO finances (user_id, amount, description, type)"
        " VALUES ($1::integer, $2::double precision, $3, $4::smallint) RETURNING id",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Finance::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    Finance created = finance;
    created.setId(newId);
    return created;
}

std::optional<Finance> PostgresExpenseRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, amount, description, type FROM finances WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Finance::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;
    return rowToFinance(res.get(), 0);
}

std::vector<Finance> PostgresExpenseRepository::findByUserId(uint32_t userId) {
    std::string uid = std::to_string(userId);
    const char* p[] = { uid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, amount, description, type FROM finances WHERE user_id = $1 ORDER BY id",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Finance::findByUserId");

    std::vector<Finance> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) out.push_back(rowToFinance(res.get(), i));
    return out;
}

bool PostgresExpenseRepository::update(const Finance& finance) {
    std::string sid    = std::to_string(finance.getId());
    std::string amount = std::to_string(finance.getAmount());
    std::string desc   = finance.getDescription();
    std::string type   = std::to_string(static_cast<int>(finance.getType()));

    const char* p[] = { amount.c_str(), desc.c_str(), type.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE finances SET amount=$1::double precision, description=$2, type=$3::smallint WHERE id=$4",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "Finance::update");
    return pg::rowsAffected(res.get());
}

bool PostgresExpenseRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM finances WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "Finance::remove");
    return pg::rowsAffected(res.get());
}
