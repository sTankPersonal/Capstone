#pragma once
#include "infrastructureServices/apiClient/IPromptBuilder.h"
#include "Transactions.h"
#include <string>
#include <vector>

class PromptBuilder : public IPromptBuilder {
    std::string              userMessage_;
    std::vector<Transaction> transactions_;

public:
    PromptBuilder& withUserMessage(const std::string& message) override;
    PromptBuilder& withTransactionContext(const std::vector<Transaction>& transactions) override;
    std::string build() override;
};
