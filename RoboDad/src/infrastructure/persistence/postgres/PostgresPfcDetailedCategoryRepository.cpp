#include "infrastructure/persistence/postgres/PostgresPfcDetailedCategoryRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresPfcDetailedCategoryRepository::PostgresPfcDetailedCategoryRepository(DatabaseConnection& db) : db_(db) {}

static PfcDetailedCategory rowToDetailed(const pqxx::row& row) {
    return PfcDetailedCategory{
        PfcDetailedCategoryId{row["pfc_detailed_category_id"].as<std::string>()},
        PfcPrimaryCategoryId{row["pfc_primary_category_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

static const char* kSelect =
    "SELECT pfc_detailed_category_id, pfc_primary_category_id, value, created_at FROM pfc_detailed_categories";

PfcDetailedCategory PostgresPfcDetailedCategoryRepository::create(const PfcDetailedCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO pfc_detailed_categories(pfc_detailed_category_id, pfc_primary_category_id, value, created_at) "
        "VALUES($1,$2,$3,$4)",
        entity.getId().getId(),
        entity.getPrimaryCategoryId().getId(),
        entity.getValue(),
        dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<PfcDetailedCategory> PostgresPfcDetailedCategoryRepository::findById(PfcDetailedCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelect) + " WHERE pfc_detailed_category_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToDetailed(r[0]);
}

std::vector<PfcDetailedCategory> PostgresPfcDetailedCategoryRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(std::string(kSelect) + " ORDER BY pfc_primary_category_id, pfc_detailed_category_id");
    txn.commit();
    std::vector<PfcDetailedCategory> results;
    for (const auto& row : r) results.push_back(rowToDetailed(row));
    return results;
}

bool PostgresPfcDetailedCategoryRepository::update(const PfcDetailedCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE pfc_detailed_categories SET pfc_primary_category_id=$2, value=$3 "
        "WHERE pfc_detailed_category_id=$1",
        entity.getId().getId(),
        entity.getPrimaryCategoryId().getId(),
        entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresPfcDetailedCategoryRepository::remove(PfcDetailedCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM pfc_detailed_categories WHERE pfc_detailed_category_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::vector<PfcDetailedCategory> PostgresPfcDetailedCategoryRepository::findByPrimaryId(
    const PfcPrimaryCategoryId& primaryId)
{
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelect) + " WHERE pfc_primary_category_id=$1 ORDER BY pfc_detailed_category_id",
        primaryId.getId());
    txn.commit();
    std::vector<PfcDetailedCategory> results;
    for (const auto& row : r) results.push_back(rowToDetailed(row));
    return results;
}

std::optional<PfcDetailedCategory> PostgresPfcDetailedCategoryRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(std::string(kSelect) + " WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToDetailed(r[0]);
}
