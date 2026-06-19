#pragma once

#include "IIdValueObject.h"
#include <string>

class PlaidItemId : public IIdValueObject<std::string> {
    std::string id_;

public:
    PlaidItemId() = default;
    explicit PlaidItemId(const std::string& id);

    const std::string& getId() const noexcept override;
};
