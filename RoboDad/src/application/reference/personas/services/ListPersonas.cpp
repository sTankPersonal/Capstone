#include "application/references/personas/services/ListPersonas.h"

ListPersonas::ListPersonas(ILlmPersonaRepository& repo) : repo_(repo) {}

std::vector<PersonaDto> ListPersonas::execute(const ListPersonasQuery&) {
    auto personas = repo_.findAll();
    std::vector<PersonaDto> dtos;
    dtos.reserve(personas.size());
    for (const auto& p : personas) dtos.emplace_back(p);
    return dtos;
}
