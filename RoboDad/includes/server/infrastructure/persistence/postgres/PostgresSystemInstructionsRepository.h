#pragma once
#include "domain/repositories/SystemInstructionsRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresSystemInstructionsRepository : public SystemInstructionsRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresSystemInstructionsRepository(DatabaseConnection& db);

    SystemInstructions create(const SystemInstructions& si) override;
    std::optional<SystemInstructions> findById(uint32_t id) override;
    bool update(const SystemInstructions& si) override;
    bool remove(uint32_t id) override;
};
