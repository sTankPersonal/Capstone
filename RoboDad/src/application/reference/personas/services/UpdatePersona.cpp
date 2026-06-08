#include "application/references/personas/services/UpdatePersona.h"
#include <chrono>

UpdatePersona::UpdatePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

bool UpdatePersona::execute(const UpdatePersonaCommand& request) {
    auto persona = repo_.findById(request.id);
    if (!persona) return false;
    LlmPersona updated(request.id, request.name, request.description, request.systemPrompt, persona->getCreatedAt());
    return repo_.update(updated);
}
