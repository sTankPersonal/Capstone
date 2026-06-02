#include "LlmPersona.h"

LlmPersona::LlmPersona(
    const LlmPersonaId& id,
    const std::string& name,
    const std::string& description,
    const std::string& systemPrompt,
    const std::chrono::year_month_day& createdAt
) : id_(id), name_(name), description_(description), systemPrompt_(systemPrompt), createdAt_(createdAt) {}

const LlmPersonaId& LlmPersona::getId() const noexcept { return id_; }
const std::string& LlmPersona::getName() const noexcept { return name_; }
const std::string& LlmPersona::getDescription() const noexcept { return description_; }
const std::string& LlmPersona::getSystemPrompt() const noexcept { return systemPrompt_; }
const std::chrono::year_month_day& LlmPersona::getCreatedAt() const noexcept { return createdAt_; }

void LlmPersona::setName(const std::string& name) { name_ = name; }
void LlmPersona::setDescription(const std::string& description) { description_ = description; }
void LlmPersona::setSystemPrompt(const std::string& systemPrompt) { systemPrompt_ = systemPrompt; }
