#pragma once
#include "UserId.h"

class GetFinancialInsightsQuery {
public:
    UserId userId;

    explicit GetFinancialInsightsQuery(const UserId& id)
        : userId(id) {
    }
};
