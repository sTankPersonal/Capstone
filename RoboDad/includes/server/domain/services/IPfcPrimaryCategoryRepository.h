#pragma once
#include "IRepository.h"
#include "PfcPrimaryCategory.h"
#include "PfcPrimaryCategoryId.h"
#include <optional>
#include <string>

class IPfcPrimaryCategoryRepository : public IRepository<PfcPrimaryCategory, PfcPrimaryCategoryId> {
public:
    virtual std::optional<PfcPrimaryCategory> findByValue(const std::string& value) = 0;
};
