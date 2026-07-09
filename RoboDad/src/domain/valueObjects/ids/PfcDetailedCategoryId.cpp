#include "PfcDetailedCategoryId.h"

PfcDetailedCategoryId::PfcDetailedCategoryId(const std::string& id) : id_(id) {}

const std::string& PfcDetailedCategoryId::getId() const noexcept { return id_; }
