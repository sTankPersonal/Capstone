#pragma once
#include "IDto.h"
#include "Language.h"
#include <string>
#include "crow.h"

class LanguageDto : public IDto {
    std::string id_;
    std::string value_;
    std::string createdAt_;

public:
    explicit LanguageDto(const Language& language);
    explicit operator crow::json::wvalue() const override;
};
