#include "UserInformation.h"

UserInformation::UserInformation(
    const std::optional<std::string>& firstName,
    const std::optional<std::string>& lastName,
    const std::optional<std::chrono::year_month_day>& dateOfBirth,
    const std::optional<CountryId>& countryId,
    const std::optional<CurrencyId>& currencyId,
    const std::optional<LanguageId>& languageId,
    const std::optional<EmploymentStatusId>& employmentStatusId
) : firstName_(firstName), lastName_(lastName), dateOfBirth_(dateOfBirth), countryId_(countryId), currencyId_(currencyId), languageId_(languageId), employmentStatusId_(employmentStatusId) {}

const std::optional<std::string>& UserInformation::getFirstName() const noexcept { return firstName_; }
const std::optional<std::string>& UserInformation::getLastName() const noexcept { return lastName_; }
const std::optional<std::chrono::year_month_day>& UserInformation::getDateOfBirth() const noexcept { return dateOfBirth_; }
const std::optional<CountryId>& UserInformation::getCountryId() const noexcept { return countryId_; }
const std::optional<CurrencyId>& UserInformation::getCurrencyId() const noexcept { return currencyId_; }
const std::optional<LanguageId>& UserInformation::getLanguageId() const noexcept { return languageId_; }
const std::optional<EmploymentStatusId>& UserInformation::getEmploymentStatusId() const noexcept { return employmentStatusId_; }

void UserInformation::setFirstName(const std::optional<std::string>& firstName) { firstName_ = firstName; }
void UserInformation::setLastName(const std::optional<std::string>& lastName) { lastName_ = lastName; }
void UserInformation::setDateOfBirth(const std::optional<std::chrono::year_month_day>& dateOfBirth) { dateOfBirth_ = dateOfBirth; }
void UserInformation::setCountryId(const std::optional<CountryId>& countryId) { countryId_ = countryId; }
void UserInformation::setCurrencyId(const std::optional<CurrencyId>& currencyId) { currencyId_ = currencyId; }
void UserInformation::setLanguageId(const std::optional<LanguageId>& languageId) { languageId_ = languageId; }
void UserInformation::setEmploymentStatusId(const std::optional<EmploymentStatusId>& employmentStatusId) { employmentStatusId_ = employmentStatusId; }
