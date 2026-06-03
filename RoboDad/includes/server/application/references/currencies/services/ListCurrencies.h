#pragma once
#include "IUseCase.h"
#include "ListCurrenciesQuery.h"
#include "ICurrencyRepository.h"
#include "CurrencyDto.h"
#include <vector>

class ListCurrencies : public IUseCase<ListCurrenciesQuery, std::vector<CurrencyDto>> {
    ICurrencyRepository& repo_;
public:
    explicit ListCurrencies(ICurrencyRepository& repo);
    std::vector<CurrencyDto> execute(const ListCurrenciesQuery& request) override;
};
