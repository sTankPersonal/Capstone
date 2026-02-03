#pragma once

#include <compare>

template<typename TId, typename TCreatedAt>
class IEntity {
public:
    virtual const TId& getId() const noexcept = 0;
    virtual const TCreatedAt& getCreatedAt() const noexcept = 0;
    virtual ~IEntity() = default;
    virtual std::strong_ordering operator<=>(const IEntity&) const noexcept = default;
};
