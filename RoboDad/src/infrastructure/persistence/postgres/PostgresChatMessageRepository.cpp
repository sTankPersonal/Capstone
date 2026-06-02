#include "infrastructure/persistence/postgres/PostgresChatMessageRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresChatMessageRepository::PostgresChatMessageRepository(DatabaseConnection& db) : db_(db) {}

static ChatMessage rowToMessage(const pqxx::row& row) {
    std::optional<std::string> content = row["content"].is_null()
        ? std::nullopt : std::make_optional(row["content"].as<std::string>());

    return ChatMessage{
        ChatMessageId{row["chat_message_id"].as<std::string>()},
        ChatSessionId{row["chat_session_id"].as<std::string>()},
        MessageSenderId{row["message_sender_id"].as<std::string>()},
        ChatMessageContent{content},
        dateFromStr(row["created_at"].as<std::string>())
    };
}

static const char* kSelectCols =
    "SELECT chat_message_id, chat_session_id, message_sender_id, content, created_at "
    "FROM chat_messages";

ChatMessage PostgresChatMessageRepository::create(const ChatMessage& message) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO chat_messages(chat_message_id, chat_session_id, message_sender_id, "
        "content, created_at) VALUES($1,$2,$3,$4,$5)",
        message.getId().getId(),
        message.getChatSessionId().getId(),
        message.getMessageSenderId().getId(),
        message.getContent().getContent(),
        dateToStr(message.getCreatedAt())
    );
    txn.commit();
    return message;
}

std::optional<ChatMessage> PostgresChatMessageRepository::findById(ChatMessageId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE chat_message_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToMessage(r[0]);
}

std::vector<ChatMessage> PostgresChatMessageRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(kSelectCols);
    txn.commit();
    std::vector<ChatMessage> results;
    for (const auto& row : r) results.push_back(rowToMessage(row));
    return results;
}

bool PostgresChatMessageRepository::update(const ChatMessage& message) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE chat_messages SET content=$2 WHERE chat_message_id=$1",
        message.getId().getId(),
        message.getContent().getContent()
    );
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresChatMessageRepository::remove(ChatMessageId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM chat_messages WHERE chat_message_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::vector<ChatMessage> PostgresChatMessageRepository::findByChatSessionId(const ChatSessionId& sessionId) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE chat_session_id=$1 ORDER BY created_at",
        sessionId.getId());
    txn.commit();
    std::vector<ChatMessage> results;
    for (const auto& row : r) results.push_back(rowToMessage(row));
    return results;
}

std::vector<ChatMessage> PostgresChatMessageRepository::findByChatSessionId(const ChatSessionId& sessionId, int limit) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE chat_session_id=$1 ORDER BY created_at DESC LIMIT $2",
        sessionId.getId(), limit);
    txn.commit();
    std::vector<ChatMessage> results;
    for (const auto& row : r) results.push_back(rowToMessage(row));
    std::reverse(results.begin(), results.end());
    return results;
}