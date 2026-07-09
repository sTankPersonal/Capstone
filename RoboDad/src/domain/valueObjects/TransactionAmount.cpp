#include "TransactionAmount.h"
#include <cmath>

namespace {
    std::optional<double> roundToCent(const std::optional<double>& amount) {
        if (!amount) return amount;
        return std::round(*amount * 100.0) / 100.0;
    }
}

TransactionAmount::TransactionAmount(const std::optional<double>& amount, const std::optional<CurrencyId>& currencyId)
    : amount_(roundToCent(amount)), currencyId_(currencyId) {}

const std::optional<double>& TransactionAmount::getAmount() const noexcept { return amount_; }
const std::optional<CurrencyId>& TransactionAmount::getCurrencyId() const noexcept { return currencyId_; }

void TransactionAmount::setAmount(const std::optional<double>& amount) { amount_ = roundToCent(amount); }
void TransactionAmount::setCurrencyId(const std::optional<CurrencyId>& currencyId) { currencyId_ = currencyId; }
