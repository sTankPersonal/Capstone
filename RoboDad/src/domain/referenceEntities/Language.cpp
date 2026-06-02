#include "Language.h"

Language::Language(const LanguageId& id, const std::string& value, const std::chrono::year_month_day& createdAt)
    : id_(id), value_(value), createdAt_(createdAt) {}

const LanguageId& Language::getId() const noexcept { return id_; }
const std::string& Language::getValue() const noexcept { return value_; }
const std::chrono::year_month_day& Language::getCreatedAt() const noexcept { return createdAt_; }

void Language::setValue(const std::string& value) { value_ = value; }
