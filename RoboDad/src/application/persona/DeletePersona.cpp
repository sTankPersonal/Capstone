#include "application/persona/DeletePersona.h"

DeletePersona::DeletePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

bool DeletePersona::execute(const LlmPersonaId& id) {
    return repo_.remove(id);
}
