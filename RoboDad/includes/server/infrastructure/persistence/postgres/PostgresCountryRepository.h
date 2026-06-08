#pragma once

#include "ICountryRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresCountryRepository : public ICountryRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresCountryRepository(DatabaseConnection& db);

    Country create(const Country& entity) override;
    std::optional<Country> findById(CountryId id) override;
    std::vector<Country> findAll() override;
    bool update(const Country& entity) override;
    bool remove(CountryId id) override;

    std::optional<Country> findByValue(const std::string& value);
};
