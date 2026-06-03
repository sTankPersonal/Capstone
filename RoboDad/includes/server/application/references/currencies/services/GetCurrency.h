#pragma once
#include "IUseCase.h"
#include "GetCurrencyQuery.h"
#include "ICurrencyRepository.h"
#include "CurrencyDto.h"
#include <optional>

class GetCurrency : public IUseCase<GetCurrencyQuery, std::optional<CurrencyDto>> {
    ICurrencyRepository& repo_;
public:
    explicit GetCurrency(ICurrencyRepository& repo);
    std::optional<CurrencyDto> execute(const GetCurrencyQuery& request) override;
};
