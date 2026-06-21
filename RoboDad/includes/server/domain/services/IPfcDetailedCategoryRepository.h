#pragma once
#include "IRepository.h"
#include "PfcDetailedCategory.h"
#include "PfcDetailedCategoryId.h"
#include "PfcPrimaryCategoryId.h"
#include <optional>
#include <string>
#include <vector>

class IPfcDetailedCategoryRepository : public IRepository<PfcDetailedCategory, PfcDetailedCategoryId> {
public:
    virtual std::vector<PfcDetailedCategory> findByPrimaryId(const PfcPrimaryCategoryId& primaryId) = 0;
    virtual std::optional<PfcDetailedCategory> findByValue(const std::string& value) = 0;
};
