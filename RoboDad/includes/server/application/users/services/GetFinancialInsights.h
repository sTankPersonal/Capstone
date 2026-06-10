#pragma once
#include "IUseCase.h"
#include "GetFinancialInsightsQuery.h"
#include "FinancialInsightsDto.h"
#include "ITransactionRepository.h"
#include <optional>

class GetFinancialInsights
    : public IUseCase<GetFinancialInsightsQuery, std::optional<FinancialInsightsDto>>
{
    ITransactionRepository& repo_;

public:
    explicit GetFinancialInsights(ITransactionRepository& repo);
    std::optional<FinancialInsightsDto> execute(const GetFinancialInsightsQuery& request) override;
};
