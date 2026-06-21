#include "PfcDetailedCategory.h"

PfcDetailedCategory::PfcDetailedCategory(const PfcDetailedCategoryId& id,
                                         const PfcPrimaryCategoryId& primaryCategoryId,
                                         const std::string& value,
                                         const std::chrono::year_month_day& createdAt)
    : id_(id), primaryCategoryId_(primaryCategoryId), value_(value), createdAt_(createdAt) {}

const PfcDetailedCategoryId& PfcDetailedCategory::getId() const noexcept { return id_; }
const PfcPrimaryCategoryId&  PfcDetailedCategory::getPrimaryCategoryId() const noexcept { return primaryCategoryId_; }
const std::string&           PfcDetailedCategory::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& PfcDetailedCategory::getCreatedAt() const noexcept { return createdAt_; }
