#include "TransactionCategoryId.h"

TransactionCategoryId::TransactionCategoryId(const std::string& id) : id_(id) {}

const std::string& TransactionCategoryId::getId() const noexcept { return id_; }
