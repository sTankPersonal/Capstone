#pragma once
#include "IUseCase.h"
#include "ListCountriesQuery.h"
#include "ICountryRepository.h"
#include "CountryDto.h"
#include <vector>

class ListCountries : public IUseCase<ListCountriesQuery, std::vector<CountryDto>> {
    ICountryRepository& repo_;
public:
    explicit ListCountries(ICountryRepository& repo);
    std::vector<CountryDto> execute(const ListCountriesQuery& request) override;
};
