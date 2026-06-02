#pragma once

#include "ILlmPersonaRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresLlmPersonaRepository : public ILlmPersonaRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresLlmPersonaRepository(DatabaseConnection& db);

    LlmPersona create(const LlmPersona& entity) override;
    std::optional<LlmPersona> findById(LlmPersonaId id) override;
    std::vector<LlmPersona> findAll() override;
    bool update(const LlmPersona& entity) override;
    bool remove(LlmPersonaId id) override;

    std::optional<LlmPersona> findByName(const std::string& name);
};
