#include "CountryId.h"

CountryId::CountryId(const std::string& id) : id_(id) {}

const std::string& CountryId::getId() const noexcept { return id_; }
