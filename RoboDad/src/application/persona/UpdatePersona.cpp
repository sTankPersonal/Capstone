#include "application/persona/UpdatePersona.h"
#include <chrono>

UpdatePersona::UpdatePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

bool UpdatePersona::execute(const LlmPersonaId& id,
                             const std::string&  name,
                             const std::string&  description,
                             const std::string&  systemPrompt) {
    auto persona = repo_.findById(id);
    if (!persona) return false;
    LlmPersona updated(id, name, description, systemPrompt, persona->getCreatedAt());
    return repo_.update(updated);
}
