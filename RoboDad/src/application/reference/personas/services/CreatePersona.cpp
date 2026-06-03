#include "application/references/personas/services/CreatePersona.h"
#include "LlmPersonaId.h"
#include "UuidGenerator.h"
#include <chrono>

CreatePersona::CreatePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

PersonaDto CreatePersona::execute(const CreatePersonaCommand& request) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    LlmPersona persona(
        LlmPersonaId(UuidGenerator::generate()),
        request.name,
        request.description,
        request.systemPrompt,
        today
    );
    return PersonaDto(repo_.create(persona));
}
