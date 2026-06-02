#pragma once

#include "IValueObject.h"
#include "EmploymentStatusId.h"
#include "CountryId.h"
#include "CurrencyId.h"
#include "LanguageId.h"
#include <chrono>
#include <string>
#include <optional>

class UserInformation : public IValueObject {
    std::optional<std::string> firstName_;
    std::optional<std::string> lastName_;
    std::optional<std::chrono::year_month_day> dateOfBirth_;
    std::optional<CountryId> countryId_;
    std::optional<CurrencyId> currencyId_;
    std::optional<LanguageId> languageId_;
    std::optional<EmploymentStatusId> employmentStatusId_;

public:
    UserInformation() = default;
    UserInformation(
        const std::optional<std::string>& firstName,
        const std::optional<std::string>& lastName,
        const std::optional<std::chrono::year_month_day>& dateOfBirth,
        const std::optional<CountryId>& countryId,
        const std::optional<CurrencyId>& currencyId,
        const std::optional<LanguageId>& languageId,
        const std::optional<EmploymentStatusId>& employmentStatusId
    );

    const std::optional<std::string>& getFirstName() const noexcept;
    const std::optional<std::string>& getLastName() const noexcept;
    const std::optional<std::chrono::year_month_day>& getDateOfBirth() const noexcept;
    const std::optional<CountryId>& getCountryId() const noexcept;
    const std::optional<CurrencyId>& getCurrencyId() const noexcept;
    const std::optional<LanguageId>& getLanguageId() const noexcept;
    const std::optional<EmploymentStatusId>& getEmploymentStatusId() const noexcept;

protected:
    void setFirstName(const std::optional<std::string>& firstName);
    void setLastName(const std::optional<std::string>& lastName);
    void setDateOfBirth(const std::optional<std::chrono::year_month_day>& dateOfBirth);
    void setCountryId(const std::optional<CountryId>& countryId);
    void setCurrencyId(const std::optional<CurrencyId>& currencyId);
    void setLanguageId(const std::optional<LanguageId>& languageId);
    void setEmploymentStatusId(const std::optional<EmploymentStatusId>& employmentStatusId);
};
