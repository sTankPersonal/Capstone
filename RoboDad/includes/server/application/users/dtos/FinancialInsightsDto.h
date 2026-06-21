#pragma once
#include "IDto.h"
#include <unordered_map>
#include <vector>
#include <string>
#include "crow.h"

struct CategoryItem {
    std::string category;
    double amount;
    int count;
};

class FinancialInsightsDto : public IDto {
public:
    int timeSpan = 30;
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    std::vector<std::string> unusualIncome;
    std::vector<std::string> unusualExpenses;
    std::vector<CategoryItem> incomeByCategoryList;
    std::vector<CategoryItem> expenseByCategoryList;

    explicit operator crow::json::wvalue() const override;
};
