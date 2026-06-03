#pragma once
#include "IDto.h"
#include "Currency.h"
#include <string>
#include "crow.h"

class CurrencyDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit CurrencyDto(const Currency& currency);
    explicit operator crow::json::wvalue() const override;
};
