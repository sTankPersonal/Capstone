#include "TransactionDescription.h"

TransactionDescription::TransactionDescription(const std::optional<std::string>& description) : description_(description) {}

const std::optional<std::string>& TransactionDescription::getDescription() const noexcept { return description_; }

void TransactionDescription::setDescription(const std::optional<std::string>& description) { description_ = description; }
