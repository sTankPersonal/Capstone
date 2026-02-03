#include "infrastructure/persistence/postgres/PostgresCurrencyRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresCurrencyRepository::PostgresCurrencyRepository(DatabaseConnection& db) : db_(db) {}

static Currency rowToCurrency(const pqxx::row& row) {
    return Currency{
        CurrencyId{row["currency_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

Currency PostgresCurrencyRepository::create(const Currency& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO currencies(currency_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<Currency> PostgresCurrencyRepository::findById(CurrencyId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT currency_id, value, created_at FROM currencies WHERE currency_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCurrency(r[0]);
}

std::vector<Currency> PostgresCurrencyRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec("SELECT currency_id, value, created_at FROM currencies ORDER BY value");
    txn.commit();
    std::vector<Currency> results;
    for (const auto& row : r) results.push_back(rowToCurrency(row));
    return results;
}

bool PostgresCurrencyRepository::update(const Currency& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE currencies SET value=$2 WHERE currency_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresCurrencyRepository::remove(CurrencyId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params("DELETE FROM currencies WHERE currency_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<Currency> PostgresCurrencyRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT currency_id, value, created_at FROM currencies WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCurrency(r[0]);
}
