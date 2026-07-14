#pragma once
#include "IDto.h"
#include <string>
#include <optional>
#include "crow.h"

class UserSettingsViewDto : public IDto {
public:
    std::string id;
    std::optional<std::string> firstName;
    std::optional<std::string> lastName;
    std::optional<std::string> dateOfBirth;
    std::optional<std::string> countryName;
    std::optional<std::string> currencyName;
    std::optional<std::string> languageName;
    std::optional<std::string> employmentStatusName;

    explicit operator crow::json::wvalue() const override;
};
