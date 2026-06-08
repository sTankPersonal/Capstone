#pragma once
#include "IDto.h"
#include "LlmPersona.h"
#include <string>
#include "crow.h"

class PersonaDto : public IDto {
    std::string id_;
    std::string name_;
    std::string description_;
    std::string systemPrompt_;
    std::string createdAt_;

public:
    explicit PersonaDto(const LlmPersona& persona);
    explicit operator crow::json::wvalue() const override;
};
