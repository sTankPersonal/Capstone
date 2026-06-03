#pragma once
#include "IUseCase.h"
#include "GetCountryQuery.h"
#include "ICountryRepository.h"
#include "CountryDto.h"
#include <optional>

class GetCountry : public IUseCase<GetCountryQuery, std::optional<CountryDto>> {
    ICountryRepository& repo_;
public:
    explicit GetCountry(ICountryRepository& repo);
    std::optional<CountryDto> execute(const GetCountryQuery& request) override;
};
