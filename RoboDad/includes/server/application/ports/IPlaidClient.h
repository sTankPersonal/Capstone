#pragma once

#include "PlaidSyncResult.h"
#include <string>

class IPlaidClient {
public:
    virtual ~IPlaidClient() = default;
    virtual std::string createLinkToken(const std::string& userId) = 0;
    virtual std::string exchangePublicToken(const std::string& publicToken) = 0;
    virtual std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") = 0;
    virtual PlaidSyncResult fetchTransactions(const std::string& accessToken,
                                              const std::string& cursor = "") = 0;
};
