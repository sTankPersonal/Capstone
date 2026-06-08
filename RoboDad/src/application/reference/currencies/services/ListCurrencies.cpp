#include "application/references/currencies/services/ListCurrencies.h"

ListCurrencies::ListCurrencies(ICurrencyRepository& repo) : repo_(repo) {}

std::vector<CurrencyDto> ListCurrencies::execute(const ListCurrenciesQuery&) {
    auto items = repo_.findAll();
    std::vector<CurrencyDto> dtos;
    dtos.reserve(items.size());
    for (const auto& c : items) dtos.emplace_back(c);
    return dtos;
}
