#include "application/users/services/GetUserSettingsView.h"
#include <chrono>

static std::string pad2(unsigned n) {
    std::string s = std::to_string(n);
    return s.size() < 2 ? "0" + s : s;
}

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + pad2(unsigned(d.month())) + "-"
         + pad2(unsigned(d.day()));
}

GetUserSettingsView::GetUserSettingsView(
    IUserRepository& userRepo,
    ICountryRepository& countryRepo,
    ICurrencyRepository& currencyRepo,
    ILanguageRepository& languageRepo,
    IEmploymentStatusRepository& employmentStatusRepo
) : userRepo_(userRepo), countryRepo_(countryRepo), currencyRepo_(currencyRepo), languageRepo_(languageRepo), employmentStatusRepo_(employmentStatusRepo) {}

std::optional<UserSettingsViewDto> GetUserSettingsView::execute(const GetUserProfileQuery& request) {
    auto user = userRepo_.findById(request.userId);
    if (!user) return std::nullopt;

    const auto& info = user->getUserInformation();

    UserSettingsViewDto dto;
    dto.id = user->getId().getId();
    if (info.getFirstName())   dto.firstName   = *info.getFirstName();
    if (info.getLastName())    dto.lastName    = *info.getLastName();
    if (info.getDateOfBirth()) dto.dateOfBirth = formatDate(*info.getDateOfBirth());

    if (info.getCountryId())          dto.countryName          = countryRepo_.findById(*info.getCountryId())->getValue();
    if (info.getCurrencyId())         dto.currencyName         = currencyRepo_.findById(*info.getCurrencyId())->getValue();
    if (info.getLanguageId())         dto.languageName         = languageRepo_.findById(*info.getLanguageId())->getValue();
    if (info.getEmploymentStatusId()) dto.employmentStatusName = employmentStatusRepo_.findById(*info.getEmploymentStatusId())->getValue();

    return dto;
}
