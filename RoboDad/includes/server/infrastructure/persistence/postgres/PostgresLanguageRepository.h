#pragma once

#include "ILanguageRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresLanguageRepository : public ILanguageRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresLanguageRepository(DatabaseConnection& db);

    Language create(const Language& entity) override;
    std::optional<Language> findById(LanguageId id) override;
    std::vector<Language> findAll() override;
    bool update(const Language& entity) override;
    bool remove(LanguageId id) override;

    std::optional<Language> findByValue(const std::string& value);
};
