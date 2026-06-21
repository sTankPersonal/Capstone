#pragma once
#include "IUseCase.h"
#include "GetFinancialInsightsQuery.h"
#include "FinancialInsightsDto.h"
#include "ITransactionRepository.h"
#include "IPfcDetailedCategoryRepository.h"
#include <optional>

class GetFinancialInsights
    : public IUseCase<GetFinancialInsightsQuery, std::optional<FinancialInsightsDto>>
{
    ITransactionRepository&         repo_;
    IPfcDetailedCategoryRepository& pfcDetailedRepo_;

public:
    GetFinancialInsights(ITransactionRepository& repo, IPfcDetailedCategoryRepository& pfcDetailedRepo);
    std::optional<FinancialInsightsDto> execute(const GetFinancialInsightsQuery& request) override;
};
