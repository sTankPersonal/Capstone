#pragma once
#include <string>
#include <vector>
#include "finance/Finance.hpp"

class IPlaidClient {
public:
    virtual ~IPlaidClient() = default;
    virtual std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") = 0;
    virtual std::vector<Finance> fetchTransactionsAsFinances(const std::string& accessToken,
                                                             const std::string& startDate,
                                                             const std::string& endDate,
                                                             int count = 100) = 0;
};
