#pragma once
#include "IRepository.h"
#include "Currency.h"
#include "CurrencyId.h"
#include <optional>
#include <string>

class ICurrencyRepository : public IRepository<Currency, CurrencyId> {
public:
    virtual std::optional<Currency> findByValue(const std::string& value) = 0;
};
