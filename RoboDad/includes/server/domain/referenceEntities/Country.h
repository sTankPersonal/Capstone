#pragma once

#include "IEntity.h"
#include "CountryId.h"
#include <chrono>
#include <string>

class Country : public IEntity<CountryId, std::chrono::year_month_day>
{
    CountryId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit Country(const CountryId& id, const std::string& value, const std::chrono::year_month_day& createdAt);

    const CountryId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setValue(const std::string& value);
};
