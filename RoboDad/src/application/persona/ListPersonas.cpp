#include "application/persona/ListPersonas.h"

ListPersonas::ListPersonas(ILlmPersonaRepository& repo) : repo_(repo) {}

std::vector<LlmPersona> ListPersonas::execute() {
    return repo_.findAll();
}
