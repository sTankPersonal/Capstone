#pragma once
#include "domain/IEntity.h"

template<typename TId = uint32_t>
class IAggregateRoot : public IEntity<TId> {};
