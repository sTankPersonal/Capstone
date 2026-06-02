#pragma once

#include "IValueObject.h"
#include <string>
#include <optional>

class TransactionDescription : public IValueObject {
    std::optional<std::string> description_;

public:
    TransactionDescription() = default;
    explicit TransactionDescription(const std::optional<std::string>& description);

    const std::optional<std::string>& getDescription() const noexcept;

protected:
    void setDescription(const std::optional<std::string>& description);
};
