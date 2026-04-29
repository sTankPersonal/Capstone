#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include "user/EmploymentStatus.hpp"
#include "finance/FinanceEnum.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>

namespace {

std::vector<Finance> fetchFinancesForUser(PGconn* conn, uint32_t userId) {
    std::string uid = std::to_string(userId);
    const char* p[] = { uid.c_str() };
    pg::Result res(PQexecParams(conn,
        "SELECT id, amount, description, type FROM finances WHERE user_id = $1 ORDER BY id",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, conn, "fetchFinancesForUser");

    std::vector<Finance> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) {
        uint32_t id    = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), i, 0)));
        double amount  = std::stod(PQgetvalue(res.get(), i, 1));
        std::string desc = PQgetvalue(res.get(), i, 2);
        auto type      = static_cast<FinanceEnum>(std::stoi(PQgetvalue(res.get(), i, 3)));
        out.emplace_back(id, amount, desc, type);
    }
    return out;
}

User rowToUser(PGresult* res, int row, PGconn* conn) {
    uint32_t uid = static_cast<uint32_t>(std::stoul(PQgetvalue(res, row, 0)));
    std::string name = PQgetvalue(res, row, 1);
    uint8_t age      = static_cast<uint8_t>(std::stoi(PQgetvalue(res, row, 2)));
    auto status      = static_cast<EmploymentStatus>(std::stoi(PQgetvalue(res, row, 3)));
    PersonalInfo pi(name, age, status);
    return User(uid, pi, fetchFinancesForUser(conn, uid));
}

} // namespace

PostgresUserRepository::PostgresUserRepository(DatabaseConnection& db) : db_(db) {}

User PostgresUserRepository::create(const User& user) {
    const auto& pi = user.getPersonalInfo();
    std::string name   = pi.getName();
    std::string age    = std::to_string(static_cast<int>(pi.getAge()));
    std::string status = std::to_string(static_cast<int>(pi.getEmploymentStatus()));

    const char* p[] = { name.c_str(), age.c_str(), status.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO users (name, age, employment_status)"
        " VALUES ($1, $2::smallint, $3::smallint) RETURNING id",
        3, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "User::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    return User(newId, pi, user.getFinances());
}

std::optional<User> PostgresUserRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, name, age, employment_status FROM users WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "User::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;
    return rowToUser(res.get(), 0, db_.get());
}

std::vector<User> PostgresUserRepository::findAll() {
    pg::Result res(PQexec(db_.get(),
        "SELECT id, name, age, employment_status FROM users ORDER BY id"));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "User::findAll");

    std::vector<User> users;
    int n = PQntuples(res.get());
    users.reserve(n);
    for (int i = 0; i < n; ++i)
        users.push_back(rowToUser(res.get(), i, db_.get()));
    return users;
}

bool PostgresUserRepository::update(const User& user) {
    const auto& pi = user.getPersonalInfo();
    std::string sid    = std::to_string(user.getId());
    std::string name   = pi.getName();
    std::string age    = std::to_string(static_cast<int>(pi.getAge()));
    std::string status = std::to_string(static_cast<int>(pi.getEmploymentStatus()));

    const char* p[] = { name.c_str(), age.c_str(), status.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE users SET name=$1, age=$2::smallint, employment_status=$3::smallint WHERE id=$4",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "User::update");
    return pg::rowsAffected(res.get());
}

bool PostgresUserRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM users WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "User::remove");
    return pg::rowsAffected(res.get());
}
