#include "infrastructure/persistence/postgres/PostgresChatHistoryRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>

namespace {

ChatHistory rowToChatHistory(PGresult* res, int row) {
    uint32_t id   = static_cast<uint32_t>(std::stoul(PQgetvalue(res, row, 0)));
    uint64_t ts   = static_cast<uint64_t>(std::stoull(PQgetvalue(res, row, 1)));
    std::string userMsg  = PQgetvalue(res, row, 2);
    std::string llmResp  = PQgetvalue(res, row, 3);
    return ChatHistory(id, ts, userMsg, llmResp);
}

} // namespace

PostgresChatHistoryRepository::PostgresChatHistoryRepository(DatabaseConnection& db) : db_(db) {}

ChatHistory PostgresChatHistoryRepository::create(uint32_t promptId, const ChatHistory& history) {
    std::string pid   = std::to_string(promptId);
    std::string ts    = std::to_string(history.getTimestamp());
    std::string umsg  = history.getUserMessage();
    std::string lresp = history.getLLMResponse();

    const char* p[] = { pid.c_str(), ts.c_str(), umsg.c_str(), lresp.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO chat_history (prompt_id, timestamp, user_message, llm_response)"
        " VALUES ($1::integer, $2::bigint, $3, $4) RETURNING id",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "ChatHistory::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    return ChatHistory(newId, history.getTimestamp(), history.getUserMessage(), history.getLLMResponse());
}

std::optional<ChatHistory> PostgresChatHistoryRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, timestamp, user_message, llm_response FROM chat_history WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "ChatHistory::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;
    return rowToChatHistory(res.get(), 0);
}

std::vector<ChatHistory> PostgresChatHistoryRepository::findByPromptId(uint32_t promptId) {
    std::string pid = std::to_string(promptId);
    const char* p[] = { pid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, timestamp, user_message, llm_response FROM chat_history WHERE prompt_id = $1 ORDER BY timestamp",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "ChatHistory::findByPromptId");

    std::vector<ChatHistory> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) out.push_back(rowToChatHistory(res.get(), i));
    return out;
}

bool PostgresChatHistoryRepository::update(const ChatHistory& history) {
    std::string sid   = std::to_string(history.getId());
    std::string ts    = std::to_string(history.getTimestamp());
    std::string umsg  = history.getUserMessage();
    std::string lresp = history.getLLMResponse();

    const char* p[] = { ts.c_str(), umsg.c_str(), lresp.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE chat_history SET timestamp=$1::bigint, user_message=$2, llm_response=$3 WHERE id=$4",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "ChatHistory::update");
    return pg::rowsAffected(res.get());
}

bool PostgresChatHistoryRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM chat_history WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "ChatHistory::remove");
    return pg::rowsAffected(res.get());
}
