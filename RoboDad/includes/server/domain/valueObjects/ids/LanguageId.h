#pragma once

#include "IIdValueObject.h"
#include <string>

class LanguageId : public IIdValueObject<std::string> {
    std::string id_;

public:
    LanguageId() = default;
    explicit LanguageId(const std::string& id);

    const std::string& getId() const noexcept override;
};
