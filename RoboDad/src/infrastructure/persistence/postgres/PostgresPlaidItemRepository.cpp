#include "infrastructure/persistence/postgres/PostgresPlaidItemRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresPlaidItemRepository::PostgresPlaidItemRepository(DatabaseConnection& db) : db_(db) {}

static PlaidItem rowToPlaidItem(const pqxx::row& row) {
    return PlaidItem{
        PlaidItemId{row["plaid_item_id"].as<std::string>()},
        UserId{row["user_id"].as<std::string>()},
        row["access_token"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>()),
        row["sync_cursor"].as<std::string>()
    };
}

static const char* kSelectCols =
    "SELECT plaid_item_id, user_id, access_token, sync_cursor, created_at FROM plaid_items";

PlaidItem PostgresPlaidItemRepository::create(const PlaidItem& item) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO plaid_items(plaid_item_id, user_id, access_token, sync_cursor, created_at) "
        "VALUES($1,$2,$3,$4,$5)",
        item.getId().getId(),
        item.getUserId().getId(),
        item.getAccessToken(),
        item.getSyncCursor(),
        dateToStr(item.getCreatedAt())
    );
    txn.commit();
    return item;
}

std::optional<PlaidItem> PostgresPlaidItemRepository::findById(PlaidItemId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE plaid_item_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPlaidItem(r[0]);
}

std::vector<PlaidItem> PostgresPlaidItemRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(kSelectCols);
    txn.commit();
    std::vector<PlaidItem> results;
    for (const auto& row : r) results.push_back(rowToPlaidItem(row));
    return results;
}

bool PostgresPlaidItemRepository::update(const PlaidItem& item) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE plaid_items SET access_token=$2, sync_cursor=$3 WHERE plaid_item_id=$1",
        item.getId().getId(),
        item.getAccessToken(),
        item.getSyncCursor()
    );
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresPlaidItemRepository::remove(PlaidItemId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM plaid_items WHERE plaid_item_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<PlaidItem> PostgresPlaidItemRepository::findByUserId(const UserId& userId) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE user_id=$1 ORDER BY created_at DESC LIMIT 1",
        userId.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPlaidItem(r[0]);
}
