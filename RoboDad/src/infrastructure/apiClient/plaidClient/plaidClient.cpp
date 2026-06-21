#include "infrastructure/apiClient/plaidClient/plaidClient.h"
#include "PlaidSyncResult.h"
#include <crow/json.h>
#include <stdexcept>
#include <sstream>

static std::string plaidBaseUrl(const std::string& env) {
    if (env == "production") return "https://production.plaid.com";
    return "https://sandbox.plaid.com";
}

PlaidClient::PlaidClient(const std::string& clientId, const std::string& secret,
                         const std::string& env, const std::string& redirectUri)
    : ApiClient(clientId, plaidBaseUrl(env)), secret_(secret), redirectUri_(redirectUri) {}

std::string PlaidClient::buildBody(const std::string& extraFields) const {
    return "{\"client_id\":\"" + apiKey_ + "\","
           "\"secret\":\"" + secret_ + "\","
           + extraFields + "}";
}

// ---- link token (real Plaid Link flow) -------------------------------------

std::string PlaidClient::createLinkToken(const std::string& userId) {
    std::string extra =
        "\"user\":{\"client_user_id\":\"" + userId + "\"},"
        "\"client_name\":\"RoboDad\","
        "\"products\":[\"transactions\"],"
        "\"country_codes\":[\"US\",\"CA\"],"
        "\"language\":\"en\"";
    if (!redirectUri_.empty())
        extra += ",\"redirect_uri\":\"" + redirectUri_ + "\"";
    const std::string body = buildBody(extra);
    const std::string raw = httpPost(
        baseUrl_ + "/link/token/create",
        body,
        {"Content-Type: application/json"}
    );
    auto json = crow::json::load(raw);
    if (!json || !json.has("link_token"))
        throw std::runtime_error("PlaidClient: missing link_token in response");
    return json["link_token"].s();
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

PlaidSyncResult PlaidClient::fetchTransactions(
    const std::string& accessToken,
    const std::string& cursor)
{
    PlaidSyncResult result;
    std::string currentCursor = cursor;

    for (;;) {
        std::string extra = "\"access_token\":\"" + accessToken + "\"";
        if (!currentCursor.empty())
            extra += ",\"cursor\":\"" + currentCursor + "\"";

        const std::string raw = httpPost(
            baseUrl_ + "/transactions/sync",
            buildBody(extra),
            {"Content-Type: application/json"}
        );

        auto json = crow::json::load(raw);
        if (!json)
            throw std::runtime_error("PlaidClient: invalid JSON in transactions/sync response");

        auto added    = parseTransactionArray(raw, "added");
        auto modified = parseTransactionArray(raw, "modified");
        auto removed  = parseRemovedArray(raw);

        result.added.insert(result.added.end(), added.begin(), added.end());
        result.modified.insert(result.modified.end(), modified.begin(), modified.end());
        result.removed.insert(result.removed.end(), removed.begin(), removed.end());

        bool hasMore = json.has("has_more") && json["has_more"].t() == crow::json::type::True;
        if (json.has("next_cursor"))
            currentCursor = std::string(json["next_cursor"].s());
        if (!hasMore) break;
    }

    result.nextCursor = currentCursor;
    return result;
}

std::vector<PlaidTransactionData> PlaidClient::parseTransactionArray(const std::string& jsonResponse, const std::string& arrayKey) const {
    auto json = crow::json::load(jsonResponse);
    if (!json || !json.has(arrayKey))
        return {};

    const auto& txns = json[arrayKey];
    if (txns.t() != crow::json::type::List)
        return {};

    std::vector<PlaidTransactionData> result;
    for (size_t i = 0; i < txns.size(); ++i) {
        const auto& t = txns[i];

        PlaidTransactionData data;
        data.plaidTransactionId = t.has("transaction_id") ? std::string(t["transaction_id"].s()) : "";
        data.amount      = t["amount"].d();
        if (t.has("merchant_name") && t["merchant_name"].t() != crow::json::type::Null)
            data.description = std::string(t["merchant_name"].s());
        else if (t.has("name") && t["name"].t() != crow::json::type::Null)
            data.description = std::string(t["name"].s());
        data.date        = t.has("date") && t["date"].t() != crow::json::type::Null
                               ? std::string(t["date"].s()) : "";

        // category is a nullable array in production — guard both the type and size.
        if (t.has("category") && t["category"].t() == crow::json::type::List
                && t["category"].size() > 0) {
            data.category = t["category"][0].s();
        }

        // personal_finance_category (PFCv2) — object with primary and detailed fields.
        if (t.has("personal_finance_category")
                && t["personal_finance_category"].t() == crow::json::type::Object) {
            const auto& pfc = t["personal_finance_category"];
            if (pfc.has("primary") && pfc["primary"].t() != crow::json::type::Null)
                data.pfcPrimary = std::string(pfc["primary"].s());
            if (pfc.has("detailed") && pfc["detailed"].t() != crow::json::type::Null)
                data.pfcDetailed = std::string(pfc["detailed"].s());
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

std::vector<std::string> PlaidClient::parseRemovedArray(const std::string& jsonResponse) const {
    auto json = crow::json::load(jsonResponse);
    if (!json || !json.has("removed"))
        return {};

    const auto& arr = json["removed"];
    if (arr.t() != crow::json::type::List)
        return {};

    std::vector<std::string> result;
    for (size_t i = 0; i < arr.size(); ++i) {
        const auto& item = arr[i];
        if (item.has("transaction_id"))
            result.push_back(std::string(item["transaction_id"].s()));
    }
    return result;
}
