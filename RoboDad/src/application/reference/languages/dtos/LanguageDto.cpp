#include "application/references/languages/dtos/LanguageDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

LanguageDto::LanguageDto(const Language& language)
    : id_(language.getId().getId())
    , value_(language.getValue())
    , createdAt_(formatDate(language.getCreatedAt()))
{}

LanguageDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["value"]     = value_;
    result["createdAt"] = createdAt_;
    return result;
}
