#include "application/references/currencies/services/GetCurrency.h"

GetCurrency::GetCurrency(ICurrencyRepository& repo) : repo_(repo) {}

std::optional<CurrencyDto> GetCurrency::execute(const GetCurrencyQuery& request) {
    auto currency = repo_.findById(request.id);
    if (!currency) return std::nullopt;
    return CurrencyDto(*currency);
}
