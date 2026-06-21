#pragma once

#include "IPfcPrimaryCategoryRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresPfcPrimaryCategoryRepository : public IPfcPrimaryCategoryRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresPfcPrimaryCategoryRepository(DatabaseConnection& db);

    PfcPrimaryCategory create(const PfcPrimaryCategory& entity) override;
    std::optional<PfcPrimaryCategory> findById(PfcPrimaryCategoryId id) override;
    std::vector<PfcPrimaryCategory> findAll() override;
    bool update(const PfcPrimaryCategory& entity) override;
    bool remove(PfcPrimaryCategoryId id) override;

    std::optional<PfcPrimaryCategory> findByValue(const std::string& value) override;
};
