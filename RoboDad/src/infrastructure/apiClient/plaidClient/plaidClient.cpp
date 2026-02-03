#include "infrastructure/apiClient/plaidClient/plaidClient.h"
#include <crow/json.h>
#include <stdexcept>
#include <sstream>

static constexpr const char* PLAID_SANDBOX_URL = "https://sandbox.plaid.com";

PlaidClient::PlaidClient(const std::string& clientId, const std::string& secret)
    : ApiClient(clientId, PLAID_SANDBOX_URL), secret_(secret) {}

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
    return exchangePublicToken(createSandboxPublicToken(institutionId));
}

// ---- transactions ----------------------------------------------------------

std::vector<PlaidTransactionData> PlaidClient::fetchTransactions(
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

std::vector<PlaidTransactionData> PlaidClient::parseTransactions(const std::string& jsonResponse) const {
    auto json = crow::json::load(jsonResponse);
    if (!json)
        throw std::runtime_error("PlaidClient: invalid JSON in transactions response");
    if (!json.has("transactions"))
        throw std::runtime_error("PlaidClient: missing 'transactions' in response");

    std::vector<PlaidTransactionData> result;
    const auto& txns = json["transactions"];

    for (size_t i = 0; i < txns.size(); ++i) {
        const auto& t = txns[i];

        PlaidTransactionData data;
        data.amount      = t["amount"].d();
        data.description = t["name"].s();
        data.date        = t["date"].s();

        // Primary category is the first element in Plaid's "category" array.
        if (t.has("category") && t["category"].size() > 0) {
            data.category = t["category"][0].s();
        }

        // iso_currency_code may be null for some accounts.
        if (t.has("iso_currency_code") && t["iso_currency_code"].t() != crow::json::type::Null) {
            data.currencyCode = t["iso_currency_code"].s();
        } else {
            data.currencyCode = "USD";
        }

        result.push_back(std::move(data));
    }

    return result;
}
