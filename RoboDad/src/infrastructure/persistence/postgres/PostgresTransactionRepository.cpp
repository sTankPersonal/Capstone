#include "infrastructure/persistence/postgres/PostgresTransactionRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresTransactionRepository::PostgresTransactionRepository(DatabaseConnection& db) : db_(db) {}

static Transaction rowToTransaction(const pqxx::row& row) {
    std::optional<double> amount = row["amount"].is_null()
        ? std::nullopt : std::make_optional(row["amount"].as<double>());
    std::optional<CurrencyId> currencyId = row["currency_id"].is_null()
        ? std::nullopt : std::make_optional(CurrencyId{row["currency_id"].as<std::string>()});
    std::optional<std::string> desc = row["description"].is_null()
        ? std::nullopt : std::make_optional(row["description"].as<std::string>());

    return Transaction{
        TransactionId{row["transaction_id"].as<std::string>()},
        UserId{row["user_id"].as<std::string>()},
        TransactionCategoryId{row["transaction_category_id"].as<std::string>()},
        TransactionAmount{amount, currencyId},
        TransactionDescription{desc},
        dateFromStr(row["transaction_date"].as<std::string>()),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

static const char* kSelectCols =
    "SELECT transaction_id, user_id, transaction_category_id, amount, currency_id, "
    "description, transaction_date, created_at FROM transactions";

Transaction PostgresTransactionRepository::create(const Transaction& transaction) {
    pqxx::work txn{db_.getConnection()};
    const auto& amt = transaction.getAmount();
    std::optional<std::string> currStr = amt.getCurrencyId().has_value()
        ? std::make_optional(amt.getCurrencyId()->getId()) : std::nullopt;

    txn.exec_params(
        "INSERT INTO transactions(transaction_id, user_id, transaction_category_id, "
        "amount, currency_id, description, transaction_date, created_at) "
        "VALUES($1,$2,$3,$4,$5,$6,$7,$8)",
        transaction.getId().getId(),
        transaction.getUserId().getId(),
        transaction.getCategoryId().getId(),
        amt.getAmount(),
        currStr,
        transaction.getDescription().getDescription(),
        dateToStr(transaction.getTransactionDate()),
        dateToStr(transaction.getCreatedAt())
    );
    txn.commit();
    return transaction;
}

std::optional<Transaction> PostgresTransactionRepository::findById(TransactionId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE transaction_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToTransaction(r[0]);
}

std::vector<Transaction> PostgresTransactionRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(kSelectCols);
    txn.commit();
    std::vector<Transaction> results;
    for (const auto& row : r) results.push_back(rowToTransaction(row));
    return results;
}

bool PostgresTransactionRepository::update(const Transaction& transaction) {
    pqxx::work txn{db_.getConnection()};
    const auto& amt = transaction.getAmount();
    std::optional<std::string> currStr = amt.getCurrencyId().has_value()
        ? std::make_optional(amt.getCurrencyId()->getId()) : std::nullopt;

    auto res = txn.exec_params(
        "UPDATE transactions SET transaction_category_id=$2, amount=$3, currency_id=$4, "
        "description=$5, transaction_date=$6 WHERE transaction_id=$1",
        transaction.getId().getId(),
        transaction.getCategoryId().getId(),
        amt.getAmount(),
        currStr,
        transaction.getDescription().getDescription(),
        dateToStr(transaction.getTransactionDate())
    );
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresTransactionRepository::remove(TransactionId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM transactions WHERE transaction_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::vector<Transaction> PostgresTransactionRepository::findByUserId(const UserId& userId) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE user_id=$1 ORDER BY transaction_date DESC",
        userId.getId());
    txn.commit();
    std::vector<Transaction> results;
    for (const auto& row : r) results.push_back(rowToTransaction(row));
    return results;
}
