#include "application/references/personas/services/DeletePersona.h"

DeletePersona::DeletePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

bool DeletePersona::execute(const DeletePersonaCommand& request) {
    return repo_.remove(request.id);
}
