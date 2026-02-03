#pragma once

#include "IValueObject.h"
#include "CurrencyId.h"
#include <optional>

class TransactionAmount : public IValueObject {
    std::optional<double> amount_;
    std::optional<CurrencyId> currencyId_;

public:
    TransactionAmount() = default;
    explicit TransactionAmount(const std::optional<double>& amount, const std::optional<CurrencyId>& currencyId);

    const std::optional<double>& getAmount() const noexcept;
    const std::optional<CurrencyId>& getCurrencyId() const noexcept;

protected:
    void setAmount(const std::optional<double>& amount);
    void setCurrencyId(const std::optional<CurrencyId>& currencyId);
};
