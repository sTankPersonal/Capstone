#pragma once
#include "IDto.h"
#include "User.h"
#include <string>
#include <optional>
#include "crow.h"

class UserProfileDto : public IDto {
    std::string id_;
    std::string email_;
    std::optional<std::string> firstName_;
    std::optional<std::string> lastName_;
    std::optional<std::string> dateOfBirth_;
    std::optional<std::string> countryId_;
    std::optional<std::string> currencyId_;
    std::optional<std::string> languageId_;
    std::optional<std::string> employmentStatusId_;
    std::string createdAt_;
    std::string updatedAt_;

public:
    explicit UserProfileDto(const User& user);
    explicit operator crow::json::wvalue() const override;

    std::string getId() const;
    std::optional<std::string> getFirstName() const;
    std::optional<std::string> getLastName() const;
};
