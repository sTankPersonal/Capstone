#include "infrastructure/persistence/postgres/PostgresTransactionCategoryRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresTransactionCategoryRepository::PostgresTransactionCategoryRepository(DatabaseConnection& db) : db_(db) {}

static TransactionCategory rowToCategory(const pqxx::row& row) {
    return TransactionCategory{
        TransactionCategoryId{row["transaction_category_id"].as<std::string>()},
        row["value"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

TransactionCategory PostgresTransactionCategoryRepository::create(const TransactionCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO transaction_categories(transaction_category_id, value, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getValue(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<TransactionCategory> PostgresTransactionCategoryRepository::findById(TransactionCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT transaction_category_id, value, created_at FROM transaction_categories WHERE transaction_category_id=$1",
        id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCategory(r[0]);
}

std::vector<TransactionCategory> PostgresTransactionCategoryRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(
        "SELECT transaction_category_id, value, created_at FROM transaction_categories ORDER BY value");
    txn.commit();
    std::vector<TransactionCategory> results;
    for (const auto& row : r) results.push_back(rowToCategory(row));
    return results;
}

bool PostgresTransactionCategoryRepository::update(const TransactionCategory& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE transaction_categories SET value=$2 WHERE transaction_category_id=$1",
        entity.getId().getId(), entity.getValue());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresTransactionCategoryRepository::remove(TransactionCategoryId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM transaction_categories WHERE transaction_category_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<TransactionCategory> PostgresTransactionCategoryRepository::findByValue(const std::string& value) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT transaction_category_id, value, created_at FROM transaction_categories WHERE value=$1", value);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToCategory(r[0]);
}
