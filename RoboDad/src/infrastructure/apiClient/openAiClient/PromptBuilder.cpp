#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include <sstream>

PromptBuilder& PromptBuilder::withUserMessage(const std::string& message) {
    userMessage_ = message;
    return *this;
}

PromptBuilder& PromptBuilder::withInsights(const FinancialInsightsDto& insights) {
    std::ostringstream oss;

    oss << "\nFinancial Summary (Last 30 Days):\n";

    // ---------------- Income ----------------
    oss << "\nIncome:\n";
    oss << "- Total Income: $" << insights.totalIncome << "\n";

    if (!insights.incomeByCategoryList.empty()) {
        oss << "Income by Category:\n";
        for (const auto& item : insights.incomeByCategoryList) {
            oss << "  • " << item.category << ": $" << item.amount
                << " across " << item.count << " transactions\n";
        }
    }

    if (!insights.unusualIncome.empty()) {
        oss << "Unusual Income:\n";
        for (const auto& desc : insights.unusualIncome) {
            oss << "  • " << desc << "\n";
        }
    }

    // ---------------- Expenses ----------------
    oss << "\nExpenses:\n";
    oss << "- Total Expenses: $" << insights.totalExpenses << "\n";

    if (!insights.expenseByCategoryList.empty()) {
        oss << "Expenses by Category:\n";
        for (const auto& item : insights.expenseByCategoryList) {
            oss << "  • " << item.category << ": $" << item.amount
                << " across " << item.count << " transactions\n";
        }
    }

    if (!insights.unusualExpenses.empty()) {
        oss << "Unusual Expenses:\n";
        for (const auto& desc : insights.unusualExpenses) {
            oss << "  • " << desc << "\n";
        }
    }

    userMessage_ += oss.str();
    return *this;
}

std::string PromptBuilder::build() {
    std::string result = std::move(userMessage_);
    userMessage_.clear();
    return result;
}
