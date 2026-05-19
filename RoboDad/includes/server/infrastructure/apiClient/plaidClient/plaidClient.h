#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "infrastructureServices/apiClient/IPlaidClient.h"
#include "finance/Finance.hpp"
#include <string>
#include <vector>

// Plaid sandbox client used for seeding the database with realistic finance data.
// Bypasses the browser Link flow by using /sandbox/public_token/create, which
// lets us create test access tokens programmatically.
//
// Plaid uses client_id + secret rather than a single bearer token.
// clientId_ is stored in the parent's apiKey_ field; secret_ is stored separately.
class PlaidClient : public ApiClient, public IPlaidClient {
    std::string secret_;

public:
    // clientId: Plaid client_id from the dashboard.
    // secret:   Plaid sandbox secret.
    PlaidClient(const std::string& clientId, const std::string& secret);

    // Creates a sandbox public token for the given institution (default: First Platypus Bank),
    // then immediately exchanges it for a persistent access token.
    // Call this once per seeding session; pass the returned token to fetchTransactionsAsFinances().
    // Throws std::runtime_error on HTTP error or malformed response.
    std::string createSandboxAccessToken(const std::string& institutionId = "ins_109508") override;

    // Fetches up to `count` transactions in [startDate, endDate] (format: "YYYY-MM-DD")
    // and maps each one to a Finance domain object:
    //   positive Plaid amount (debit)  → FinanceEnum::Expense
    //   negative Plaid amount (credit) → FinanceEnum::Salary
    // The returned Finance objects have id=0; callers persist them via ExpenseRepository.
    // Throws std::runtime_error on HTTP error or malformed response.
    std::vector<Finance> fetchTransactionsAsFinances(const std::string& accessToken,
                                                     const std::string& startDate,
                                                     const std::string& endDate,
                                                     int count = 100) override;

private:
    // Builds the JSON auth block shared by every Plaid request.
    std::string buildBody(const std::string& extraFields) const;

    std::string createSandboxPublicToken(const std::string& institutionId);
    std::string exchangePublicToken(const std::string& publicToken);

    std::vector<Finance> parseTransactions(const std::string& jsonResponse) const;
};
