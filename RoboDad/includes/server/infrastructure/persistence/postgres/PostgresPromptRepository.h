#pragma once
#include "domain/repositories/PromptRepository.h"
#include "infrastructure/persistence/DatabaseConnection.h"

class PostgresPromptRepository : public PromptRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresPromptRepository(DatabaseConnection& db);

    Prompt create(uint32_t userId, const Prompt& prompt) override;
    std::optional<Prompt> findById(uint32_t id) override;
    std::vector<Prompt> findByUserId(uint32_t userId) override;
    bool update(const Prompt& prompt) override;
    bool remove(uint32_t id) override;
};
