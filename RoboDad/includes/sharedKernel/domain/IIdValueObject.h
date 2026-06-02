#pragma once

#include "IValueObject.h"

template<typename TId>
class IIdValueObject : public IValueObject {
public:
    virtual const TId& getId() const noexcept = 0;

    bool operator==(const IIdValueObject<TId>& other) const {
        return getId() == other.getId();
    }

    virtual ~IIdValueObject() = default;
};