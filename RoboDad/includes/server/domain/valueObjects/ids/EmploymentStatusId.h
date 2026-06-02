#pragma once

#include "IIdValueObject.h"
#include <string>

class EmploymentStatusId : public IIdValueObject<std::string> {
    std::string id_;

public:
    EmploymentStatusId() = default;
    explicit EmploymentStatusId(const std::string& id);

    const std::string& getId() const noexcept override;
};
