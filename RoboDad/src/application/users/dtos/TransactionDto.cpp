#include "application/users/dtos/TransactionDto.h"
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

static std::string formatDate(const std::chrono::year_month_day& d) {
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(4) << int(d.year()) << "-"
        << std::setw(2) << unsigned(d.month()) << "-"
        << std::setw(2) << unsigned(d.day());
    return oss.str();
}

static std::string formatAmount(double amount) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << amount;
    return oss.str();
}

TransactionDto::TransactionDto(const Transaction& transaction)
    : id_(transaction.getId().getId())
    , userId_(transaction.getUserId().getId())
    , categoryId_(transaction.getCategoryId().getId())
    , transactionDate_(formatDate(transaction.getTransactionDate()))
    , createdAt_(formatDate(transaction.getCreatedAt()))
{
    const auto& amt = transaction.getAmount();
    if (amt.getAmount())    amount_     = *amt.getAmount();
    if (amt.getCurrencyId()) currencyId_ = amt.getCurrencyId()->getId();

    const auto& desc = transaction.getDescription();
    if (desc.getDescription()) description_ = *desc.getDescription();

    if (transaction.getPfcDetailedCategoryId())
        pfcDetailedCategoryId_ = transaction.getPfcDetailedCategoryId()->getId();
}

std::string TransactionDto::getUserId() const { return userId_; }
std::string TransactionDto::getCategoryId() const { return categoryId_; }

TransactionDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]              = id_;
    result["userId"]          = userId_;
    result["categoryId"]      = categoryId_;
    if (pfcDetailedCategoryId_) result["pfcDetailedCategoryId"] = *pfcDetailedCategoryId_;
    if (amount_)      result["amount"]     = formatAmount(*amount_);
    if (currencyId_)  result["currencyId"] = *currencyId_;
    if (description_) result["description"] = *description_;
    result["transactionDate"] = transactionDate_;
    result["createdAt"]       = createdAt_;
    return result;
}
