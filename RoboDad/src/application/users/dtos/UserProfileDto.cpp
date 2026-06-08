#include "application/users/dtos/UserProfileDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
}

UserProfileDto::UserProfileDto(const User& user)
    : id_(user.getId().getId())
    , email_(user.getUserLogin().getEmail())
    , createdAt_(formatDate(user.getCreatedAt()))
    , updatedAt_(formatDate(user.getUpdatedAt()))
{
    const auto& info = user.getUserInformation();
    if (info.getFirstName())         firstName_         = *info.getFirstName();
    if (info.getLastName())          lastName_          = *info.getLastName();
    if (info.getDateOfBirth())       dateOfBirth_       = formatDate(*info.getDateOfBirth());
    if (info.getCountryId())         countryId_         = info.getCountryId()->getId();
    if (info.getCurrencyId())        currencyId_        = info.getCurrencyId()->getId();
    if (info.getLanguageId())        languageId_        = info.getLanguageId()->getId();
    if (info.getEmploymentStatusId()) employmentStatusId_ = info.getEmploymentStatusId()->getId();
}

std::string UserProfileDto::getId() const { return id_; }
std::optional<std::string> UserProfileDto::getFirstName() const { return firstName_; }
std::optional<std::string> UserProfileDto::getLastName() const { return lastName_; }

UserProfileDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]    = id_;
    result["email"] = email_;
    if (firstName_)          result["firstName"]          = *firstName_;
    if (lastName_)           result["lastName"]           = *lastName_;
    if (dateOfBirth_)        result["dateOfBirth"]        = *dateOfBirth_;
    if (countryId_)          result["countryId"]          = *countryId_;
    if (currencyId_)         result["currencyId"]         = *currencyId_;
    if (languageId_)         result["languageId"]         = *languageId_;
    if (employmentStatusId_) result["employmentStatusId"] = *employmentStatusId_;
    result["createdAt"] = createdAt_;
    result["updatedAt"] = updatedAt_;
    return result;
}
