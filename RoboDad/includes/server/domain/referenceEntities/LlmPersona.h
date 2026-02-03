#pragma once

#include "IEntity.h"
#include "LlmPersonaId.h"
#include <chrono>
#include <string>

class LlmPersona : public IEntity<LlmPersonaId, std::chrono::year_month_day>
{
    LlmPersonaId id_;
    std::string name_;
    std::string description_;
    std::string systemPrompt_;
    std::chrono::year_month_day createdAt_;

public:
    explicit LlmPersona(
        const LlmPersonaId& id,
        const std::string& name,
        const std::string& description,
        const std::string& systemPrompt,
        const std::chrono::year_month_day& createdAt
    );

    const LlmPersonaId& getId() const noexcept override;
    const std::string& getName() const noexcept;
    const std::string& getDescription() const noexcept;
    const std::string& getSystemPrompt() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setName(const std::string& name);
    void setDescription(const std::string& description);
    void setSystemPrompt(const std::string& systemPrompt);
};
