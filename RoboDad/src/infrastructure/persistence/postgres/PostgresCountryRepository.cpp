#include "infrastructure/persistence/postgres/PostgresCountryRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresCountryRepository::PostgresCountryRepository(DatabaseConnection& db) : db_(db) {}

static Country rowToCountry(const pqxx::row& row) {
    return Country{
        CountryId{row["country_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

Country PostgresCountryRepository::create(const Country& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO countries(country_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<Country> PostgresCountryRepository::findById(CountryId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT country_id, value, created_at FROM countries WHERE country_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCountry(r[0]);
}

std::vector<Country> PostgresCountryRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec("SELECT country_id, value, created_at FROM countries ORDER BY value");
    txn.commit();
    std::vector<Country> results;
    for (const auto& row : r) results.push_back(rowToCountry(row));
    return results;
}

bool PostgresCountryRepository::update(const Country& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE countries SET value=$2 WHERE country_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresCountryRepository::remove(CountryId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params("DELETE FROM countries WHERE country_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<Country> PostgresCountryRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT country_id, value, created_at FROM countries WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCountry(r[0]);
}
