#pragma once

#include "IPfcDetailedCategoryRepository.h"
#include "DatabaseConnection.h"
#include <optional>
#include <string>
#include <vector>

class PostgresPfcDetailedCategoryRepository : public IPfcDetailedCategoryRepository {
    DatabaseConnection& db_;

public:
    explicit PostgresPfcDetailedCategoryRepository(DatabaseConnection& db);

    PfcDetailedCategory create(const PfcDetailedCategory& entity) override;
    std::optional<PfcDetailedCategory> findById(PfcDetailedCategoryId id) override;
    std::vector<PfcDetailedCategory> findAll() override;
    bool update(const PfcDetailedCategory& entity) override;
    bool remove(PfcDetailedCategoryId id) override;

    std::vector<PfcDetailedCategory> findByPrimaryId(const PfcPrimaryCategoryId& primaryId) override;
    std::optional<PfcDetailedCategory> findByValue(const std::string& value) override;
};
