#include "application/users/dtos/TransactionDto.h"
#include <chrono>
#include <string>

static std::string formatDate(const std::chrono::year_month_day& d) {
    return std::to_string(int(d.year())) + "-"
         + std::to_string(unsigned(d.month())) + "-"
         + std::to_string(unsigned(d.day()));
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
}

std::string TransactionDto::getUserId() const { return userId_; }
std::string TransactionDto::getCategoryId() const { return categoryId_; }

TransactionDto::operator crow::json::wvalue() const {
    crow::json::wvalue result;
    result["id"]              = id_;
    result["userId"]          = userId_;
    result["categoryId"]      = categoryId_;
    if (amount_)      result["amount"]     = *amount_;
    if (currencyId_)  result["currencyId"] = *currencyId_;
    if (description_) result["description"] = *description_;
    result["transactionDate"] = transactionDate_;
    result["createdAt"]       = createdAt_;
    return result;
}
