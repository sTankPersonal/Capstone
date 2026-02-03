#include "EmploymentStatus.h"

EmploymentStatus::EmploymentStatus(const EmploymentStatusId& id, const std::string& value, const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const EmploymentStatusId& EmploymentStatus::getId() const noexcept { return id_; }
const std::string& EmploymentStatus::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& EmploymentStatus::getCreatedAt() const noexcept { return createdAt_; }

void EmploymentStatus::setValue(const std::string& value) { value_ = value; }
