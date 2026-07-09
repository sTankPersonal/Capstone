#pragma once

#include "IIdValueObject.h"
#include <string>

class PfcPrimaryCategoryId : public IIdValueObject<std::string> {
    std::string id_;

public:
    PfcPrimaryCategoryId() = default;
    explicit PfcPrimaryCategoryId(const std::string& id);

    const std::string& getId() const noexcept override;
};
