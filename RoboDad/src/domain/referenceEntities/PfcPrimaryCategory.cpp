#include "PfcPrimaryCategory.h"

PfcPrimaryCategory::PfcPrimaryCategory(const PfcPrimaryCategoryId& id, const std::string& value,
                                       const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const PfcPrimaryCategoryId& PfcPrimaryCategory::getId() const noexcept { return id_; }
const std::string& PfcPrimaryCategory::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& PfcPrimaryCategory::getCreatedAt() const noexcept { return createdAt_; }
