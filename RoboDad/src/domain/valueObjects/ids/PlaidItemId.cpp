#include "PlaidItemId.h"

PlaidItemId::PlaidItemId(const std::string& id) : id_(id) {}

const std::string& PlaidItemId::getId() const noexcept { return id_; }
