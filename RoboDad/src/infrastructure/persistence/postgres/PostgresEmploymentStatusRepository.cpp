#include "infrastructure/persistence/postgres/PostgresEmploymentStatusRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresEmploymentStatusRepository::PostgresEmploymentStatusRepository(DatabaseConnection& db) : db_(db) {}

static EmploymentStatus rowToEmploymentStatus(const pqxx::row& row) {
    return EmploymentStatus{
        EmploymentStatusId{row["employment_status_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

EmploymentStatus PostgresEmploymentStatusRepository::create(const EmploymentStatus& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO employment_statuses(employment_status_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<EmploymentStatus> PostgresEmploymentStatusRepository::findById(EmploymentStatusId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT employment_status_id, value, created_at FROM employment_statuses WHERE employment_status_id=$1",
        id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToEmploymentStatus(r[0]);
}

std::vector<EmploymentStatus> PostgresEmploymentStatusRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(
        "SELECT employment_status_id, value, created_at FROM employment_statuses ORDER BY value");
    txn.commit();
    std::vector<EmploymentStatus> results;
    for (const auto& row : r) results.push_back(rowToEmploymentStatus(row));
    return results;
}

bool PostgresEmploymentStatusRepository::update(const EmploymentStatus& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE employment_statuses SET value=$2 WHERE employment_status_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresEmploymentStatusRepository::remove(EmploymentStatusId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM employment_statuses WHERE employment_status_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<EmploymentStatus> PostgresEmploymentStatusRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT employment_status_id, value, created_at FROM employment_statuses WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToEmploymentStatus(r[0]);
}
