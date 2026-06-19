#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "IPlaidClient.h"
#include "PlaidTransactionData.h"
#include <string>
#include <vector>

class PlaidClient : public ApiClient, public IPlaidClient {
    std::string secret_;
    std::string redirectUri_;

public:
    PlaidClient(const std::string& clientId, const std::string& secret,
                const std::string& env = "sandbox",
                const std::string& redirectUri = "");

    std::string createLinkToken(const std::string& userId) override;
    std::string exchangePublicToken(const std::string& publicToken) override;
    std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") override;
    PlaidSyncResult fetchTransactions(const std::string& accessToken,
                                      const std::string& cursor = "") override;

private:
    std::string buildBody(const std::string& extraFields) const;
    std::string createSandboxPublicToken(const std::string& institutionId);
    std::vector<PlaidTransactionData> parseTransactionArray(const std::string& jsonResponse, const std::string& arrayKey) const;
    std::vector<std::string>          parseRemovedArray(const std::string& jsonResponse) const;
};
