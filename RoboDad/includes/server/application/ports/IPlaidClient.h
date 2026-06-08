#pragma once

#include "PlaidTransactionData.h"
#include <string>
#include <vector>

class IPlaidClient {
public:
    virtual ~IPlaidClient() = default;
    virtual std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") = 0;
    virtual std::vector<PlaidTransactionData> fetchTransactions(const std::string& accessToken,
                                                                const std::string& startDate,
                                                                const std::string& endDate,
                                                                int count = 100) = 0;
};
