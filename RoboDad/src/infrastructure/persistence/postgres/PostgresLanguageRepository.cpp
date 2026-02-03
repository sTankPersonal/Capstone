#include "infrastructure/persistence/postgres/PostgresLanguageRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresLanguageRepository::PostgresLanguageRepository(DatabaseConnection& db) : db_(db) {}

static Language rowToLanguage(const pqxx::row& row) {
    return Language{
        LanguageId{row["language_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

Language PostgresLanguageRepository::create(const Language& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO languages(language_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<Language> PostgresLanguageRepository::findById(LanguageId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT language_id, value, created_at FROM languages WHERE language_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToLanguage(r[0]);
}

std::vector<Language> PostgresLanguageRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec("SELECT language_id, value, created_at FROM languages ORDER BY value");
    txn.commit();
    std::vector<Language> results;
    for (const auto& row : r) results.push_back(rowToLanguage(row));
    return results;
}

bool PostgresLanguageRepository::update(const Language& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE languages SET value=$2 WHERE language_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresLanguageRepository::remove(LanguageId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params("DELETE FROM languages WHERE language_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<Language> PostgresLanguageRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT language_id, value, created_at FROM languages WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToLanguage(r[0]);
}
