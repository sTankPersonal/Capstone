#pragma once

#include "IEntity.h"
#include "LanguageId.h"
#include <chrono>
#include <string>

class Language : public IEntity<LanguageId, std::chrono::year_month_day>
{
    LanguageId id_;
    std::string value_;
    std::chrono::year_month_day createdAt_;

public:
    explicit Language(const LanguageId& id, const std::string& value, const std::chrono::year_month_day& createdAt);

    const LanguageId& getId() const noexcept override;
    const std::string& getValue() const noexcept;
    const std::chrono::year_month_day& getCreatedAt() const noexcept override;

protected:
    void setValue(const std::string& value);
};
