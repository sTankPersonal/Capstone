#include "application/references/countries/services/ListCountries.h"

ListCountries::ListCountries(ICountryRepository& repo) : repo_(repo) {}

std::vector<CountryDto> ListCountries::execute(const ListCountriesQuery&) {
    auto countries = repo_.findAll();
    std::vector<CountryDto> dtos;
    dtos.reserve(countries.size());
    for (const auto& c : countries) dtos.emplace_back(c);
    return dtos;
}
