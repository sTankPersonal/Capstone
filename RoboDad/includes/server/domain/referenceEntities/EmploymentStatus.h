#pragma once

#include "IEntity.h"
#include "EmploymentStatusId.h"
#include <chrono>
#include <string>

class EmploymentStatus : public IEntity<EmploymentStatusId, std::chrono::year_month_day>
{
    EmploymentStatusId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit EmploymentStatus(const EmploymentStatusId& id, const std::string& value, const std::chrono::year_month_day& createdAt);

    const EmploymentStatusId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setValue(const std::string& value);
};
