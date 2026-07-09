#include "infrastructure/persistence/postgres/PostgresPfcPrimaryCategoryRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresPfcPrimaryCategoryRepository::PostgresPfcPrimaryCategoryRepository(DatabaseConnection& db) : db_(db) {}

static PfcPrimaryCategory rowToPrimary(const pqxx::row& row) {
    return PfcPrimaryCategory{
        PfcPrimaryCategoryId{row["pfc_primary_category_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

static const char* kSelect = "SELECT pfc_primary_category_id, value, created_at FROM pfc_primary_categories";

PfcPrimaryCategory PostgresPfcPrimaryCategoryRepository::create(const PfcPrimaryCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO pfc_primary_categories(pfc_primary_category_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<PfcPrimaryCategory> PostgresPfcPrimaryCategoryRepository::findById(PfcPrimaryCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelect) + " WHERE pfc_primary_category_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPrimary(r[0]);
}

std::vector<PfcPrimaryCategory> PostgresPfcPrimaryCategoryRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(std::string(kSelect) + " ORDER BY pfc_primary_category_id");
    txn.commit();
    std::vector<PfcPrimaryCategory> results;
    for (const auto& row : r) results.push_back(rowToPrimary(row));
    return results;
}

bool PostgresPfcPrimaryCategoryRepository::update(const PfcPrimaryCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE pfc_primary_categories SET value=$2 WHERE pfc_primary_category_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresPfcPrimaryCategoryRepository::remove(PfcPrimaryCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM pfc_primary_categories WHERE pfc_primary_category_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<PfcPrimaryCategory> PostgresPfcPrimaryCategoryRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelect) + " WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPrimary(r[0]);
}
