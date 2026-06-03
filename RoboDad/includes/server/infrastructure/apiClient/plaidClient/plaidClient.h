#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "IPlaidClient.h"
#include "PlaidTransactionData.h"
#include <string>
#include <vector>

class PlaidClient : public ApiClient, public IPlaidClient {
    std::string secret_;

public:
    PlaidClient(const std::string& clientId, const std::string& secret);
    std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") override;
    std::vector<PlaidTransactionData> fetchTransactions(const std::string& accessToken,
                                                        const std::string& startDate,
                                                        const std::string& endDate,
                                                        int count = 100) override;

private:
    std::string buildBody(const std::string& extraFields) const;
    std::string createSandboxPublicToken(const std::string& institutionId);
    std::string exchangePublicToken(const std::string& publicToken);
    std::vector<PlaidTransactionData> parseTransactions(const std::string& jsonResponse) const;
};
