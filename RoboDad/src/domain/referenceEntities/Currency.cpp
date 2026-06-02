#include "Currency.h"

Currency::Currency(const CurrencyId& id, const std::string& value, const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const CurrencyId& Currency::getId() const noexcept { return id_; }
const std::string& Currency::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& Currency::getCreatedAt() const noexcept { return createdAt_; }

void Currency::setValue(const std::string& value) { value_ = value; }
