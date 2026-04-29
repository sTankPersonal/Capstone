#include "infrastructure/persistence/postgres/PostgresPromptRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include "llm/SystemInstructions.hpp"
#include "llm/ChatHistory.hpp"
#include <libpq-fe.h>
#include <string>
#include <vector>

namespace {

std::vector<ChatHistory> fetchHistoriesForPrompt(PGconn* conn, uint32_t promptId) {
    std::string pid = std::to_string(promptId);
    const char* p[] = { pid.c_str() };
    pg::Result res(PQexecParams(conn,
        "SELECT id, timestamp, user_message, llm_response FROM chat_history WHERE prompt_id = $1 ORDER BY timestamp",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, conn, "fetchHistoriesForPrompt");

    std::vector<ChatHistory> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) {
        uint32_t id   = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), i, 0)));
        uint64_t ts   = static_cast<uint64_t>(std::stoull(PQgetvalue(res.get(), i, 1)));
        std::string umsg  = PQgetvalue(res.get(), i, 2);
        std::string lresp = PQgetvalue(res.get(), i, 3);
        out.emplace_back(id, ts, umsg, lresp);
    }
    return out;
}

} // namespace

PostgresPromptRepository::PostgresPromptRepository(DatabaseConnection& db) : db_(db) {}

Prompt PostgresPromptRepository::create(uint32_t userId, const Prompt& prompt) {
    std::string uid  = std::to_string(userId);
    std::string siid = std::to_string(prompt.getSystemInstructions().getId());
    std::string umsg = prompt.getUserMessage();

    const char* p[] = { uid.c_str(), siid.c_str(), umsg.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO prompts (user_id, system_instructions_id, user_message)"
        " VALUES ($1::integer, $2::integer, $3) RETURNING id",
        3, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Prompt::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    Prompt created = prompt;
    created.setId(newId);
    return created;
}

std::optional<Prompt> PostgresPromptRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT p.id, p.user_message, si.id, si.personality, si.safety_guidelines, si.goal"
        " FROM prompts p"
        " JOIN system_instructions si ON si.id = p.system_instructions_id"
        " WHERE p.id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Prompt::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;

    uint32_t promptId       = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    std::string userMsg     = PQgetvalue(res.get(), 0, 1);
    uint32_t siId           = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 2)));
    std::string personality = PQgetvalue(res.get(), 0, 3);
    std::string guidelines  = PQgetvalue(res.get(), 0, 4);
    std::string goal        = PQgetvalue(res.get(), 0, 5);

    SystemInstructions si(siId, personality, guidelines, goal);
    auto histories = fetchHistoriesForPrompt(db_.get(), promptId);
    return Prompt(promptId, si, histories, userMsg);
}

std::vector<Prompt> PostgresPromptRepository::findByUserId(uint32_t userId) {
    std::string uid = std::to_string(userId);
    const char* p[] = { uid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT p.id, p.user_message, si.id, si.personality, si.safety_guidelines, si.goal"
        " FROM prompts p"
        " JOIN system_instructions si ON si.id = p.system_instructions_id"
        " WHERE p.user_id = $1 ORDER BY p.id",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "Prompt::findByUserId");

    std::vector<Prompt> out;
    int n = PQntuples(res.get());
    out.reserve(n);
    for (int i = 0; i < n; ++i) {
        uint32_t promptId       = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), i, 0)));
        std::string userMsg     = PQgetvalue(res.get(), i, 1);
        uint32_t siId           = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), i, 2)));
        std::string personality = PQgetvalue(res.get(), i, 3);
        std::string guidelines  = PQgetvalue(res.get(), i, 4);
        std::string goal        = PQgetvalue(res.get(), i, 5);

        SystemInstructions si(siId, personality, guidelines, goal);
        auto histories = fetchHistoriesForPrompt(db_.get(), promptId);
        out.emplace_back(promptId, si, histories, userMsg);
    }
    return out;
}

bool PostgresPromptRepository::update(const Prompt& prompt) {
    std::string sid  = std::to_string(prompt.getId());
    std::string siid = std::to_string(prompt.getSystemInstructions().getId());
    std::string umsg = prompt.getUserMessage();

    const char* p[] = { siid.c_str(), umsg.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE prompts SET system_instructions_id=$1::integer, user_message=$2 WHERE id=$3",
        3, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "Prompt::update");
    return pg::rowsAffected(res.get());
}

bool PostgresPromptRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM prompts WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "Prompt::remove");
    return pg::rowsAffected(res.get());
}
