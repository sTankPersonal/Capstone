#include "LlmPersonaId.h"

LlmPersonaId::LlmPersonaId(const std::string& id) : id_(id) {}

const std::string& LlmPersonaId::getId() const noexcept { return id_; }
