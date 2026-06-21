#include "PfcPrimaryCategoryId.h"

PfcPrimaryCategoryId::PfcPrimaryCategoryId(const std::string& id) : id_(id) {}

const std::string& PfcPrimaryCategoryId::getId() const noexcept { return id_; }
