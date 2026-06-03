#pragma once
#include "IDto.h"
#include "Country.h"
#include <string>
#include "crow.h"

class CountryDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit CountryDto(const Country& country);
    explicit operator crow::json::wvalue() const override;
};
