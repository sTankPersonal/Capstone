#pragma once
#include "CountryId.h"

struct GetCountryQuery {
    CountryId id;

    explicit GetCountryQuery(CountryId id)
        : id(std::move(id)) {}
};
