#include "infrastructure/persistence/postgres/PostgresChatSessionRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresChatSessionRepository::PostgresChatSessionRepository(DatabaseConnection& db) : db_(db) {}

static ChatSession rowToSession(const pqxx::row& row) {
    std::optional<std::string> addInfo = row["additional_info"].is_null()
        ? std::nullopt : std::make_optional(row["additional_info"].as<std::string>());

    return ChatSession{
        ChatSessionId{row["chat_session_id"].as<std::string>()},
        UserId{row["user_id"].as<std::string>()},
        LlmPersonaId{row["llm_persona_id"].as<std::string>()},
        ChatSessionDescription{row["description"].as<std::string>(), addInfo},
        dateFromStr(row["created_at"].as<std::string>()),
        dateFromStr(row["updated_at"].as<std::string>())
    };
}

static const char* kSelectCols =
    "SELECT chat_session_id, user_id, llm_persona_id, description, additional_info, "
    "created_at, updated_at FROM chat_sessions";

ChatSession PostgresChatSessionRepository::create(const ChatSession& session) {
    pqxx::work txn{db_.getConnection()};
    const auto& desc = session.getSessionDescription();
    txn.exec_params(
        "INSERT INTO chat_sessions(chat_session_id, user_id, llm_persona_id, description, "
        "additional_info, created_at, updated_at) VALUES($1,$2,$3,$4,$5,$6,$7)",
        session.getId().getId(),
        session.getUserId().getId(),
        session.getPersonaId().getId(),
        desc.getDescription(),
        desc.getAdditionalInfo(),
        dateToStr(session.getCreatedAt()),
        dateToStr(session.getUpdatedAt())
    );
    txn.commit();
    return session;
}

std::optional<ChatSession> PostgresChatSessionRepository::findById(ChatSessionId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE chat_session_id=$1", id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToSession(r[0]);
}

std::vector<ChatSession> PostgresChatSessionRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(kSelectCols);
    txn.commit();
    std::vector<ChatSession> results;
    for (const auto& row : r) results.push_back(rowToSession(row));
    return results;
}

bool PostgresChatSessionRepository::update(const ChatSession& session) {
    pqxx::work txn{db_.getConnection()};
    const auto& desc = session.getSessionDescription();
    auto res = txn.exec_params(
        "UPDATE chat_sessions SET llm_persona_id=$2, description=$3, additional_info=$4, "
        "updated_at=$5 WHERE chat_session_id=$1",
        session.getId().getId(),
        session.getPersonaId().getId(),
        desc.getDescription(),
        desc.getAdditionalInfo(),
        dateToStr(session.getUpdatedAt())
    );
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresChatSessionRepository::remove(ChatSessionId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "DELETE FROM chat_sessions WHERE chat_session_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::vector<ChatSession> PostgresChatSessionRepository::findByUserId(const UserId& userId) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        std::string(kSelectCols) + " WHERE user_id=$1", userId.getId());
    txn.commit();
    std::vector<ChatSession> results;
    for (const auto& row : r) results.push_back(rowToSession(row));
    return results;
}
