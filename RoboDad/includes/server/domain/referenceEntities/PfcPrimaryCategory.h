#pragma once

#include "IEntity.h"
#include "PfcPrimaryCategoryId.h"
#include <chrono>
#include <string>

class PfcPrimaryCategory : public IEntity<PfcPrimaryCategoryId, std::chrono::year_month_day>
{
    PfcPrimaryCategoryId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit PfcPrimaryCategory(const PfcPrimaryCategoryId& id, const std::string& value,
                                const std::chrono::year_month_day& createdAt);

    const PfcPrimaryCategoryId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
};
