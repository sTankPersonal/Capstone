#include "LanguageId.h"

LanguageId::LanguageId(const std::string& id) : id_(id) {}

const std::string& LanguageId::getId() const noexcept { return id_; }
