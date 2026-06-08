#include "application/references/transactionCategories/dtos/TransactionCategoryDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

TransactionCategoryDto::TransactionCategoryDto(const TransactionCategory& category)
    : id_(category.getId().getId())
    , value_(category.getValue())
    , createdAt_(formatDate(category.getCreatedAt()))
{}

TransactionCategoryDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["value"]     = value_;
    result["createdAt"] = createdAt_;
    return result;
}
