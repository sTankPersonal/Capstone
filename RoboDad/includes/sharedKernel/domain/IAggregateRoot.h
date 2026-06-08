#pragma once

#include "IEntity.h"

template<typename TId, typename TCreatedAt, typename TUpdatedAt>
class IAggregateRoot : public IEntity<TId, TCreatedAt> {
public:
    virtual const TUpdatedAt& getUpdatedAt() const noexcept = 0;
    virtual ~IAggregateRoot() = default;

protected:
    virtual void setUpdatedAt(const TUpdatedAt& updatedAt) = 0;
};
