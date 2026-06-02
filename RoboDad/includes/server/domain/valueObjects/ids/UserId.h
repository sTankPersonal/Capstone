#pragma once

#include "IIdValueObject.h"
#include <string>

class UserId : public IIdValueObject<std::string> {
    std::string id_;

public:
    UserId() = default;
    explicit UserId(const std::string& id);

    const std::string& getId() const noexcept override;
};
