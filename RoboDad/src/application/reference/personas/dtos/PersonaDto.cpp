#include "application/references/personas/dtos/PersonaDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

PersonaDto::PersonaDto(const LlmPersona& persona)
    : id_(persona.getId().getId())
    , name_(persona.getName())
    , description_(persona.getDescription())
    , systemPrompt_(persona.getSystemPrompt())
    , createdAt_(formatDate(persona.getCreatedAt()))
{}

PersonaDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]           = id_;
    result["name"]         = name_;
    result["description"]  = description_;
    result["systemPrompt"] = systemPrompt_;
    result["createdAt"]    = createdAt_;
    return result;
}
