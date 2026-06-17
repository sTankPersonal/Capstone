#pragma once
#include "IPromptBuilder.h"
#include "FinancialInsightsDto.h"
#include "Transactions.h"
#include <string>
#include <vector>

class PromptBuilder : public IPromptBuilder {
    std::string              userMessage_;

public:
    PromptBuilder& withUserMessage(const std::string& message) override;
    PromptBuilder& withInsights(const FinancialInsightsDto& insights);
    std::string build() override;
};
