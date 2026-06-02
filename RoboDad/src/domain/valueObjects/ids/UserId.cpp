#include "UserId.h"

UserId::UserId(const std::string& id) : id_(id) {}

const std::string& UserId::getId() const noexcept { return id_; }
