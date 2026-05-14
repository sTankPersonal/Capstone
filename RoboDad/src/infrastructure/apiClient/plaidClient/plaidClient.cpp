#include "infrastructure/apiClient/plaidClient/plaidClient.h"
#include <crow/json.h>
#include <stdexcept>
#include <cmath>

static constexpr const char* PLAID_SANDBOX_URL = "https://sandbox.plaid.com";

PlaidClient::PlaidClient(const std::string& clientId, const std::string& secret)
    : ApiClient(clientId, PLAID_SANDBOX_URL), secret_(secret) {}

// Wraps extra fields in the common auth envelope that every Plaid endpoint requires.
std::string PlaidClient::buildBody(const std::string& extraFields) const {
    return "{\"client_id\":\"" + apiKey_ + "\","
           "\"secret\":\"" + secret_ + "\","
           + extraFields + "}";
}

// ---- sandbox token creation ------------------------------------------------

std::string PlaidClient::createSandboxPublicToken(const std::string& institutionId) {
    const std::string body = buildBody(
        "\"institution_id\":\"" + institutionId + "\","
        "\"initial_products\":[\"transactions\"],"
        "\"options\":{}"
    );

    const std::string raw = httpPost(
        baseUrl_ + "/sandbox/public_token/create",
        body,
        {"Content-Type: application/json"}
    );

    auto json = crow::json::load(raw);
    if (!json || !json.has("public_token"))
        throw std::runtime_error("PlaidClient: missing public_token in sandbox response");

    return json["public_token"].s();
}

std::string PlaidClient::exchangePublicToken(const std::string& publicToken) {
    const std::string body = buildBody(
        "\"public_token\":\"" + publicToken + "\""
    );

    const std::string raw = httpPost(
        baseUrl_ + "/item/public_token/exchange",
        body,
        {"Content-Type: application/json"}
    );

    auto json = crow::json::load(raw);
    if (!json || !json.has("access_token"))
        throw std::runtime_error("PlaidClient: missing access_token in exchange response");

    return json["access_token"].s();
}

std::string PlaidClient::createSandboxAccessToken(const std::string& institutionId) {
    const std::string publicToken = createSandboxPublicToken(institutionId);
    return exchangePublicToken(publicToken);
}

// ---- transactions ----------------------------------------------------------

std::vector<Finance> PlaidClient::fetchTransactionsAsFinances(
    const std::string& accessToken,
    const std::string& startDate,
    const std::string& endDate,
    int count)
{
    const std::string body = buildBody(
        "\"access_token\":\"" + accessToken + "\","
        "\"start_date\":\"" + startDate + "\","
        "\"end_date\":\"" + endDate + "\","
        "\"options\":{\"count\":" + std::to_string(count) + "}"
    );

    const std::string raw = httpPost(
        baseUrl_ + "/transactions/get",
        body,
        {"Content-Type: application/json"}
    );

    return parseTransactions(raw);
}

std::vector<Finance> PlaidClient::parseTransactions(const std::string& jsonResponse) const {
    auto json = crow::json::load(jsonResponse);
    if (!json)
        throw std::runtime_error("PlaidClient: invalid JSON in transactions response");

    if (!json.has("transactions"))
        throw std::runtime_error("PlaidClient: missing 'transactions' in response");

    std::vector<Finance> result;
    const auto& txns = json["transactions"];

    for (size_t i = 0; i < txns.size(); ++i) {
        const auto& t = txns[i];

        const double plaidAmount = t["amount"].d();
        const std::string name   = t["name"].s();

        // Plaid convention: positive = money leaving account (debit/expense),
        //                   negative = money entering account (credit/income).
        const FinanceEnum type = (plaidAmount >= 0.0) ? FinanceEnum::Expense
                                                      : FinanceEnum::Salary;
        const double amount = std::abs(plaidAmount);

        result.emplace_back(amount, name, type);
    }

    return result;
}
