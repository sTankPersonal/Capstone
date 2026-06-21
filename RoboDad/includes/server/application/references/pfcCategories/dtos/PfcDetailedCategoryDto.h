#pragma once
#include "IDto.h"
#include "PfcDetailedCategory.h"
#include <string>
#include "crow.h"

class PfcDetailedCategoryDto : public IDto {
    std::string id_;
    std::string primaryCategoryId_;
    std::string value_;
    std::string createdAt_;

public:
    explicit PfcDetailedCategoryDto(const PfcDetailedCategory& category);
    explicit operator crow::json::wvalue() const override;

    const std::string& getId() const { return id_; }
    const std::string& getPrimaryCategoryId() const { return primaryCategoryId_; }
    const std::string& getValue() const { return value_; }
};
