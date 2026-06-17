#pragma once
#include <unordered_map>
#include <vector>
#include <string>

struct CategoryItem {
    std::string category;
    double amount;
    int count;
};

class FinancialInsightsDto {
public:
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    std::vector<std::string> unusualIncome;
    std::vector<std::string> unusualExpenses;
    std::vector<CategoryItem> incomeByCategoryList;
    std::vector<CategoryItem> expenseByCategoryList;
};
