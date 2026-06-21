#include "GetFinancialInsights.h"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <chrono>
#include <unordered_map>

// ---------- Helpers ----------
static std::string categoryNameFor(
    IPfcDetailedCategoryRepository& pfcDetailedRepo,
    const std::optional<PfcDetailedCategoryId>& pfcDetailedCategoryId)
{
    if (!pfcDetailedCategoryId) return "Uncategorized";

    if (auto category = pfcDetailedRepo.findById(*pfcDetailedCategoryId); category)
        return category->getValue();

    return pfcDetailedCategoryId->getId();
}

static double computeMedian(std::vector<double> v) {
    if (v.empty()) return 0.0;
    std::sort(v.begin(), v.end());
    size_t n = v.size();
    return (n % 2 == 0)
        ? (v[n / 2 - 1] + v[n / 2]) / 2.0
        : v[n / 2];
}

static double computeMAD(const std::vector<double>& v, double median) {
    std::vector<double> deviations;
    deviations.reserve(v.size());

    for (double x : v)
        deviations.push_back(std::abs(x - median));

    return computeMedian(deviations);
}

static std::chrono::sys_days toSysDays(const std::chrono::year_month_day& ymd) {
    using namespace std::chrono;
    return sys_days{ ymd };
}

// ---------- Main logic ----------
GetFinancialInsights::GetFinancialInsights(ITransactionRepository& repo, IPfcDetailedCategoryRepository& pfcDetailedRepo)
    : repo_(repo), pfcDetailedRepo_(pfcDetailedRepo) {
}

std::optional<FinancialInsightsDto>
GetFinancialInsights::execute(const GetFinancialInsightsQuery& request)
{
    auto allTxs = repo_.findByUserId(request.userId);
    if (allTxs.empty())
        return std::nullopt;

    using namespace std::chrono;

    sys_days today = floor<days>(system_clock::now());

    // Use the parameter instead of constant 30
    int timeSpan = request.timeSpan > 0 ? request.timeSpan : 30;
    sys_days cutoff = today - days(timeSpan);

    std::vector<Transaction> txs;
    txs.reserve(allTxs.size());

    for (const auto& t : allTxs) {
        sys_days txDate = toSysDays(t.getTransactionDate());

        // "Last N days" = strictly after cutoff and up to today
        if (txDate > cutoff && txDate <= today) {
            txs.push_back(t);
        }
    }

    if (txs.empty())
        return std::nullopt;

    FinancialInsightsDto dto;

    std::unordered_map<std::string, double> incomeTotals;
    std::unordered_map<std::string, int> incomeCounts;

    std::unordered_map<std::string, double> expenseTotals;
    std::unordered_map<std::string, int> expenseCounts;

    std::vector<double> incomeAmounts;
    std::vector<double> expenseAmounts;

    // ---------- Aggregate ----------
    for (const auto& t : txs) {
        double amt = t.getAmount().getAmount().value_or(0.0);
        std::string type = t.getCategoryId().getId();
        std::string category = categoryNameFor(pfcDetailedRepo_, t.getPfcDetailedCategoryId());

        if (type == "earnings") {
            dto.totalIncome += amt;
            incomeTotals[category] += amt;
            incomeCounts[category] += 1;
            incomeAmounts.push_back(amt);
        }
        else if (type == "expenses") {
            dto.totalExpenses += amt;
            expenseTotals[category] += amt;
            expenseCounts[category] += 1;
            expenseAmounts.push_back(amt);
        }
    }

    // ---------- Convert maps → vectors for Mustache ----------
    for (const auto& [cat, amount] : incomeTotals) {
        dto.incomeByCategoryList.push_back(CategoryItem{
            cat,
            amount,
            incomeCounts[cat]
            });
    }

    for (const auto& [cat, amount] : expenseTotals) {
        dto.expenseByCategoryList.push_back(CategoryItem{
            cat,
            amount,
            expenseCounts[cat]
            });
    }

    // ---------- Unusual income ----------
    if (!incomeAmounts.empty()) {
        double median = computeMedian(incomeAmounts);
        double mad = computeMAD(incomeAmounts, median);
        if (mad == 0.0) mad = 1e-9;

        double threshold = median + 3 * mad;

        for (const auto& t : txs) {
            double amt = t.getAmount().getAmount().value_or(0.0);
            if (t.getCategoryId().getId() == "earnings" && amt > threshold) {
                dto.unusualIncome.push_back(
                    t.getDescription().getDescription().value_or("(no description)")
                );
            }
        }
    }

    // ---------- Unusual expenses ----------
    if (!expenseAmounts.empty()) {
        double median = computeMedian(expenseAmounts);
        double mad = computeMAD(expenseAmounts, median);
        if (mad == 0.0) mad = 1e-9;

        double threshold = median + 3 * mad;

        for (const auto& t : txs) {
            double amt = t.getAmount().getAmount().value_or(0.0);
            if (t.getCategoryId().getId() == "expenses" && amt > threshold) {
                dto.unusualExpenses.push_back(
                    t.getDescription().getDescription().value_or("(no description)")
                );
            }
        }
    }

    return dto;
}
