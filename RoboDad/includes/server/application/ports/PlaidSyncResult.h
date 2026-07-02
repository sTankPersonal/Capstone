#pragma once

#include "PlaidTransactionData.h"
#include <string>
#include <vector>

struct PlaidSyncResult {
    std::vector<PlaidTransactionData> added;
    std::vector<PlaidTransactionData> modified;
    std::vector<std::string>          removed;   // Plaid transaction IDs
    std::string                       nextCursor;
};
