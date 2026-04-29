#include "infrastructure/persistence/postgres/PostgresSystemInstructionsRepository.h"
#include "infrastructure/persistence/postgres/PgUtils.hpp"
#include <libpq-fe.h>
#include <string>

PostgresSystemInstructionsRepository::PostgresSystemInstructionsRepository(DatabaseConnection& db) : db_(db) {}

SystemInstructions PostgresSystemInstructionsRepository::create(const SystemInstructions& si) {
    std::string personality = si.getPersonality();
    std::string guidelines  = si.getSafetyGuidelines();
    std::string goal        = si.getGoal();

    const char* p[] = { personality.c_str(), guidelines.c_str(), goal.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "INSERT INTO system_instructions (personality, safety_guidelines, goal)"
        " VALUES ($1, $2, $3) RETURNING id",
        3, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "SystemInstructions::create");

    uint32_t newId = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    return SystemInstructions(newId, personality, guidelines, goal);
}

std::optional<SystemInstructions> PostgresSystemInstructionsRepository::findById(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "SELECT id, personality, safety_guidelines, goal FROM system_instructions WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_TUPLES_OK, db_.get(), "SystemInstructions::findById");

    if (PQntuples(res.get()) == 0) return std::nullopt;

    uint32_t uid        = static_cast<uint32_t>(std::stoul(PQgetvalue(res.get(), 0, 0)));
    std::string personality = PQgetvalue(res.get(), 0, 1);
    std::string guidelines  = PQgetvalue(res.get(), 0, 2);
    std::string goal        = PQgetvalue(res.get(), 0, 3);
    return SystemInstructions(uid, personality, guidelines, goal);
}

bool PostgresSystemInstructionsRepository::update(const SystemInstructions& si) {
    std::string sid         = std::to_string(si.getId());
    std::string personality = si.getPersonality();
    std::string guidelines  = si.getSafetyGuidelines();
    std::string goal        = si.getGoal();

    const char* p[] = { personality.c_str(), guidelines.c_str(), goal.c_str(), sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "UPDATE system_instructions SET personality=$1, safety_guidelines=$2, goal=$3 WHERE id=$4",
        4, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "SystemInstructions::update");
    return pg::rowsAffected(res.get());
}

bool PostgresSystemInstructionsRepository::remove(uint32_t id) {
    std::string sid = std::to_string(id);
    const char* p[] = { sid.c_str() };
    pg::Result res(PQexecParams(db_.get(),
        "DELETE FROM system_instructions WHERE id = $1",
        1, nullptr, p, nullptr, nullptr, 0));
    pg::check(res.get(), PGRES_COMMAND_OK, db_.get(), "SystemInstructions::remove");
    return pg::rowsAffected(res.get());
}
