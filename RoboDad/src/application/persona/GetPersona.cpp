#include "application/persona/GetPersona.h"

GetPersona::GetPersona(ILlmPersonaRepository& repo) : repo_(repo) {}

std::optional<LlmPersona> GetPersona::execute(const LlmPersonaId& id) {
    return repo_.findById(id);
}
