#include "application/users/dtos/FinancialInsightsDto.h"
#include <sstream>
#include <iomanip>

static std::string formatAmount(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

static crow::json::wvalue::list toJsonList(const std::vector<CategoryItem>& items) {
    crow::json::wvalue::list list;
    for (const auto& item : items) {
        crow::json::wvalue entry;
        entry["category"] = item.category;
        entry["amount"]   = formatAmount(item.amount);
        entry["count"]    = item.count;
        list.push_back(std::move(entry));
    }
    return list;
}

static crow::json::wvalue::list toJsonList(const std::vector<std::string>& items) {
    crow::json::wvalue::list list;
    for (const auto& item : items) {
        list.push_back(item);
    }
    return list;
}

FinancialInsightsDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["totalIncome"]           = formatAmount(totalIncome);
    result["totalExpenses"]         = formatAmount(totalExpenses);
    result["incomeByCategoryList"]  = toJsonList(incomeByCategoryList);
    result["expenseByCategoryList"] = toJsonList(expenseByCategoryList);
    result["unusualIncome"]         = toJsonList(unusualIncome);
    result["unusualExpenses"]       = toJsonList(unusualExpenses);
    return result;
}
