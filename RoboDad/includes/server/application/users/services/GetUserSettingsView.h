#pragma once
#include "IUseCase.h"
#include "GetUserProfileQuery.h"
#include "IUserRepository.h"
#include "ICountryRepository.h"
#include "ICurrencyRepository.h"
#include "ILanguageRepository.h"
#include "IEmploymentStatusRepository.h"
#include "UserSettingsViewDto.h"
#include <optional>

class GetUserSettingsView : public IUseCase<GetUserProfileQuery, std::optional<UserSettingsViewDto>> {
    IUserRepository&             userRepo_;
    ICountryRepository&          countryRepo_;
    ICurrencyRepository&         currencyRepo_;
    ILanguageRepository&         languageRepo_;
    IEmploymentStatusRepository& employmentStatusRepo_;

public:
    GetUserSettingsView(
        IUserRepository& userRepo,
        ICountryRepository& countryRepo,
        ICurrencyRepository& currencyRepo,
        ILanguageRepository& languageRepo,
        IEmploymentStatusRepository& employmentStatusRepo
    );

    std::optional<UserSettingsViewDto> execute(const GetUserProfileQuery& request) override;
};
