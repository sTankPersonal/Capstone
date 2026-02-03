#pragma once

#include "ICurrencyRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresCurrencyRepository : public ICurrencyRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresCurrencyRepository(DatabaseConnection& db);

    Currency create(const Currency& entity) override;
    std::optional<Currency> findById(CurrencyId id) override;
    std::vector<Currency> findAll() override;
    bool update(const Currency& entity) override;
    bool remove(CurrencyId id) override;

    std::optional<Currency> findByValue(const std::string& value);
};
