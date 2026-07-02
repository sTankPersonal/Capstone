#pragma once

#include "IRepository.h"
#include "PlaidItem.h"
#include "PlaidItemId.h"
#include "UserId.h"
#include <optional>

class IPlaidItemRepository : public IRepository<PlaidItem, PlaidItemId> {
public:
    virtual std::optional<PlaidItem> findByUserId(const UserId& userId) = 0;
};
