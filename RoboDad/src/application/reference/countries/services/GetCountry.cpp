#include "application/references/countries/services/GetCountry.h"

GetCountry::GetCountry(ICountryRepository& repo) : repo_(repo) {}

std::optional<CountryDto> GetCountry::execute(const GetCountryQuery& request) {
    auto country = repo_.findById(request.id);
    if (!country) return std::nullopt;
    return CountryDto(*country);
}
