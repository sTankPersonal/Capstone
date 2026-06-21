#pragma once

#include "IEntity.h"
#include "PfcDetailedCategoryId.h"
#include "PfcPrimaryCategoryId.h"
#include <chrono>
#include <string>

class PfcDetailedCategory : public IEntity<PfcDetailedCategoryId, std::chrono::year_month_day>
{
    PfcDetailedCategoryId  id_;
    PfcPrimaryCategoryId   primaryCategoryId_;
    std::string            value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit PfcDetailedCategory(const PfcDetailedCategoryId& id,
                                 const PfcPrimaryCategoryId& primaryCategoryId,
                                 const std::string& value,
                                 const std::chrono::year_month_day& createdAt);

    const PfcDetailedCategoryId& getId() const noexcept override;
    const PfcPrimaryCategoryId&  getPrimaryCategoryId() const noexcept;
    const std::string&           getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;
};
