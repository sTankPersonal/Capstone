#pragma once

#include "IIdValueObject.h"
#include <string>

class TransactionCategoryId : public IIdValueObject<std::string> {
    std::string id_;

public:
    TransactionCategoryId() = default;
    explicit TransactionCategoryId(const std::string& id);

    const std::string& getId() const noexcept override;
};
