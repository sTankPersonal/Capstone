#include "application/references/currencies/dtos/CurrencyDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

CurrencyDto::CurrencyDto(const Currency& currency)
    : id_(currency.getId().getId())
    , value_(currency.getValue())
    , createdAt_(formatDate(currency.getCreatedAt()))
{}

CurrencyDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]        = id_;
    result["value"]     = value_;
    result["createdAt"] = createdAt_;
    return result;
}
