#pragma once

#include "Transactions.h"
#include "FinancialInsightsDto.h"
#include <string>
#include <vector>

class IPromptBuilder {
public:
    virtual ~IPromptBuilder() = default;
    virtual IPromptBuilder& withUserMessage(const std::string& message) = 0;
    virtual IPromptBuilder& withInsights(const FinancialInsightsDto& insights) = 0;
    virtual std::string build() = 0;
};
