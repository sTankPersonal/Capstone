#pragma once
#include "IDto.h"
#include "PfcPrimaryCategory.h"
#include <string>
#include "crow.h"

class PfcPrimaryCategoryDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit PfcPrimaryCategoryDto(const PfcPrimaryCategory& category);
    explicit operator crow::json::wvalue() const override;

    const std::string& getId() const { return id_; }
    const std::string& getValue() const { return value_; }
};
