#pragma once
#include <cstdint>

template<typename TId = uint32_t>
class IEntity {
public:
    virtual ~IEntity() = default;
    virtual TId getId() const = 0;
};
