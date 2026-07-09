#include "application/references/pfcCategories/dtos/PfcDetailedCategoryDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

PfcDetailedCategoryDto::PfcDetailedCategoryDto(const PfcDetailedCategory& category)
    : id_(category.getId().getId())
    , primaryCategoryId_(category.getPrimaryCategoryId().getId())
    , value_(category.getValue())
    , createdAt_(formatDate(category.getCreatedAt()))
{}

PfcDetailedCategoryDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]                = id_;
    result["primaryCategoryId"] = primaryCategoryId_;
    result["value"]             = value_;
    result["createdAt"]         = createdAt_;
    return result;
}
