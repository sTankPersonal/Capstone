#pragma once

#include "IIdValueObject.h"
#include <string>

class CountryId : public IIdValueObject<std::string> {
    std::string id_;

public:
    CountryId() = default;
    explicit CountryId(const std::string& id);

    const std::string& getId() const noexcept override;
};
