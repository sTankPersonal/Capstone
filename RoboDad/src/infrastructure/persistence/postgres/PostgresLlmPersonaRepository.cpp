#include "infrastructure/persistence/postgres/PostgresLlmPersonaRepository.h"
#include "DateUtils.h"
#include <pqxx/pqxx>

PostgresLlmPersonaRepository::PostgresLlmPersonaRepository(DatabaseConnection& db) : db_(db) {}

static LlmPersona rowToPersona(const pqxx::row& row) {
    return LlmPersona{
        LlmPersonaId{row["llm_persona_id"].as<std::string>()},
        row["name"].as<std::string>(),
        row["description"].as<std::string>(),
        row["system_prompt"].as<std::string>(),
        dateFromStr(row["created_at"].as<std::string>())
    };
}

LlmPersona PostgresLlmPersonaRepository::create(const LlmPersona& entity) {
    pqxx::work txn{db_.getConnection()};
    txn.exec_params(
        "INSERT INTO llm_personas(llm_persona_id, name, description, system_prompt, created_at) VALUES($1,$2,$3,$4,$5)",
        entity.getId().getId(), entity.getName(), entity.getDescription(),
        entity.getSystemPrompt(), dateToStr(entity.getCreatedAt()));
    txn.commit();
    return entity;
}

std::optional<LlmPersona> PostgresLlmPersonaRepository::findById(LlmPersonaId id) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT llm_persona_id, name, description, system_prompt, created_at FROM llm_personas WHERE llm_persona_id=$1",
        id.getId());
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPersona(r[0]);
}

std::vector<LlmPersona> PostgresLlmPersonaRepository::findAll() {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec(
        "SELECT llm_persona_id, name, description, system_prompt, created_at FROM llm_personas ORDER BY name");
    txn.commit();
    std::vector<LlmPersona> results;
    for (const auto& row : r) results.push_back(rowToPersona(row));
    return results;
}

bool PostgresLlmPersonaRepository::update(const LlmPersona& entity) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params(
        "UPDATE llm_personas SET name=$2, description=$3, system_prompt=$4 WHERE llm_persona_id=$1",
        entity.getId().getId(), entity.getName(), entity.getDescription(), entity.getSystemPrompt());
    txn.commit();
    return res.affected_rows() > 0;
}

bool PostgresLlmPersonaRepository::remove(LlmPersonaId id) {
    pqxx::work txn{db_.getConnection()};
    auto res = txn.exec_params("DELETE FROM llm_personas WHERE llm_persona_id=$1", id.getId());
    txn.commit();
    return res.affected_rows() > 0;
}

std::optional<LlmPersona> PostgresLlmPersonaRepository::findByName(const std::string& name) {
    pqxx::work txn{db_.getConnection()};
    auto r = txn.exec_params(
        "SELECT llm_persona_id, name, description, system_prompt, created_at FROM llm_personas WHERE name=$1", name);
    txn.commit();
    if (r.empty()) return std::nullopt;
    return rowToPersona(r[0]);
}
