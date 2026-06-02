#include "TransactionId.h"

TransactionId::TransactionId(const std::string& id) : id_(id) {}

const std::string& TransactionId::getId() const noexcept { return id_; }
