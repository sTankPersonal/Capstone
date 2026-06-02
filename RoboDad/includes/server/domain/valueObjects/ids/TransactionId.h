#pragma once

#include "IIdValueObject.h"
#include <string>

class TransactionId : public IIdValueObject<std::string> {
    std::string id_;

public:
    TransactionId() = default;
    explicit TransactionId(const std::string& id);

    const std::string& getId() const noexcept override;
};
