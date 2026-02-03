#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include <sstream>

PromptBuilder& PromptBuilder::withUserMessage(const std::string& message) {
    userMessage_ = message;
    return *this;
}

PromptBuilder& PromptBuilder::withTransactionContext(const std::vector<Transaction>& transactions) {
    if (transactions.empty()) return *this;
    std::ostringstream oss;
    oss << "\nRecent Transactions:\n";
    for (const auto& tx : transactions) {
        const double   amount = tx.getAmount().getAmount().value_or(0.0);
        const std::string desc = tx.getDescription().getDescription().value_or(std::string{});
        oss << "- $" << amount << " " << desc << "\n";
    }
    userMessage_ += oss.str();
    return *this;
}

std::string PromptBuilder::build() {
    std::string result = std::move(userMessage_);
    userMessage_.clear();
    transactions_.clear();
    return result;
}
