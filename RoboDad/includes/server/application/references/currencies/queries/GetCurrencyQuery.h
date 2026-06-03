#pragma once
#include "CurrencyId.h"

struct GetCurrencyQuery {
    CurrencyId id;

    explicit GetCurrencyQuery(CurrencyId id)
        : id(std::move(id)) {}
};
