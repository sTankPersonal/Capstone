#pragma once

#include "IIdValueObject.h"
#include <string>

class CurrencyId : public IIdValueObject<std::string> {
    std::string id_;

public:
    CurrencyId() = default;
    explicit CurrencyId(const std::string& id);

    const std::string& getId() const noexcept override;
};
