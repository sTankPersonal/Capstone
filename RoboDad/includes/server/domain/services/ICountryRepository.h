#pragma once
#include "IRepository.h"
#include "Country.h"
#include "CountryId.h"
#include <optional>
#include <string>

class ICountryRepository : public IRepository<Country, CountryId> {
public:
    virtual std::optional<Country> findByValue(const std::string& value) = 0;
};
