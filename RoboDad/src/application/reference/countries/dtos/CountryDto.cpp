#include "application/references/countries/dtos/CountryDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

CountryDto::CountryDto(const Country& country)
    : id_(country.getId().getId())
    , value_(country.getValue())
    , createdAt_(formatDate(country.getCreatedAt()))
{}

CountryDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["value"]     = value_;
    result["createdAt"] = createdAt_;
    return result;
}
