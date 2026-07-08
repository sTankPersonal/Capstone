#pragma once
#include "UserId.h"

class GetFinancialInsightsQuery {
public:
    UserId userId;
    int timeSpan;

    explicit GetFinancialInsightsQuery(const UserId& id, int time)
        : userId(id), timeSpan(time) {
    }
};
