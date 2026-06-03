#include "application/references/personas/services/GetPersona.h"

GetPersona::GetPersona(ILlmPersonaRepository& repo) : repo_(repo) {}

std::optional<PersonaDto> GetPersona::execute(const GetPersonaQuery& request) {
    auto persona = repo_.findById(request.id);
    if (!persona) return std::nullopt;
    return PersonaDto(*persona);
}
