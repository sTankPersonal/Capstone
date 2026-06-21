#pragma once

#include "IIdValueObject.h"
#include <string>

class PfcDetailedCategoryId : public IIdValueObject<std::string> {
    std::string id_;

public:
    PfcDetailedCategoryId() = default;
    explicit PfcDetailedCategoryId(const std::string& id);

    const std::string& getId() const noexcept override;
};
