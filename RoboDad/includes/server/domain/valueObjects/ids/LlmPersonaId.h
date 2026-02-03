#pragma once

#include "IIdValueObject.h"
#include <string>

class LlmPersonaId : public IIdValueObject<std::string> {
    std::string id_;

public:
    LlmPersonaId() = default;
    explicit LlmPersonaId(const std::string& id);

    const std::string& getId() const noexcept override;
};
