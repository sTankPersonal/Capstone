#include "infrastructure/persistence/postgres/PostgresMessageSenderRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresMessageSenderRepository::PostgresMessageSenderRepository(DatabaseConnection& db) : db_(db) {}

static MessageSender rowToSender(const pqxx::row& row) {
    return MessageSender{
        MessageSenderId{row["message_sender_id"].as<std::string>()},
        row["name"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

MessageSender PostgresMessageSenderRepository::create(const MessageSender& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO message_senders(message_sender_id, name, created_at) VALUES($1,$2,$3)",
        entity.getId().getId(), entity.getName(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<MessageSender> PostgresMessageSenderRepository::findById(MessageSenderId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT message_sender_id, name, created_at FROM message_senders WHERE message_sender_id=$1",
        id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToSender(r[0]);
}

std::vector<MessageSender> PostgresMessageSenderRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(
        "SELECT message_sender_id, name, created_at FROM message_senders ORDER BY name");
    txn.commit();
    std::vector<MessageSender> results;
    for (const auto& row : r) results.push_back(rowToSender(row));
    return results;
}

bool PostgresMessageSenderRepository::update(const MessageSender& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE message_senders SET name=$2 WHERE message_sender_id=$1",
        entity.getId().getId(), entity.getName());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresMessageSenderRepository::remove(MessageSenderId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM message_senders WHERE message_sender_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<MessageSender> PostgresMessageSenderRepository::findByName(const std::string& name) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT message_sender_id, name, created_at FROM message_senders WHERE name=$1", name);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToSender(r[0]);
}
