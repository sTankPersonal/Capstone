#include "CurrencyId.h"

CurrencyId::CurrencyId(const std::string& id) : id_(id) {}

const std::string& CurrencyId::getId() const noexcept { return id_; }
