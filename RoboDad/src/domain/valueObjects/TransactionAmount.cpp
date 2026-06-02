#include "TransactionAmount.h"

TransactionAmount::TransactionAmount(const std::optional<double>& amount, const std::optional<CurrencyId>& currencyId)
    : amount_(amount), currencyId_(currencyId) {}

const std::optional<double>& TransactionAmount::getAmount() const noexcept { return amount_; }
const std::optional<CurrencyId>& TransactionAmount::getCurrencyId() const noexcept { return currencyId_; }

void TransactionAmount::setAmount(const std::optional<double>& amount) { amount_ = amount; }
void TransactionAmount::setCurrencyId(const std::optional<CurrencyId>& currencyId) { currencyId_ = currencyId; }
