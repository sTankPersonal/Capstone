#include "Country.h"

Country::Country(const CountryId& id, const std::string& value, const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const CountryId& Country::getId() const noexcept { return id_; }
const std::string& Country::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& Country::getCreatedAt() const noexcept { return createdAt_; }

void Country::setValue(const std::string& value) { value_ = value; }
