#pragma once

#include "IEntity.h"
#include "CurrencyId.h"
#include <chrono>
#include <string>

class Currency : public IEntity<CurrencyId, std::chrono::year_month_day>
{
    CurrencyId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit Currency(const CurrencyId& id, const std::string& value, const std::chrono::year_month_day& createdAt);

    const CurrencyId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setValue(const std::string& value);
};
