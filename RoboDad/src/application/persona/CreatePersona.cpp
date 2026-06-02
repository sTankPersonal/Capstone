#include "application/persona/CreatePersona.h"
#include "LlmPersonaId.h"
#include "UuidGenerator.h"
#include <chrono>

CreatePersona::CreatePersona(ILlmPersonaRepository& repo) : repo_(repo) {}

LlmPersona CreatePersona::execute(const std::string& name,
                                   const std::string& description,
                                   const std::string& systemPrompt) {
    const auto today = std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())
    };
    LlmPersona persona(
        LlmPersonaId(UuidGenerator::generate()),
        name,
        description,
        systemPrompt,
        today
    );
    return repo_.create(persona);
}
