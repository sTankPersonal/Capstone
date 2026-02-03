#pragma once
#include "Transactions.h"
#include <string>
#include <vector>

class IPromptBuilder {
public:
    virtual ~IPromptBuilder() = default;
    virtual IPromptBuilder& withUserMessage(const std::string& message) = 0;
    virtual IPromptBuilder& withTransactionContext(const std::vector<Transaction>& transactions) = 0;
    virtual std::string build() = 0;
};
