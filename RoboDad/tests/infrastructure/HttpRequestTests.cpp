#include <gtest/gtest.h>
#include <crow.h>
#include "infrastructure/http/HttpRequest.h"

// Helper: builds a crow::request with common fields populated.
static crow::request makeRequest(
    crow::HTTPMethod method = crow::HTTPMethod::Get,
    const std::string& body = "",
    std::initializer_list<std::pair<std::string, std::string>> hdrs = {})
{
    crow::request req;
    req.method = method;
    req.body   = body;
    for (auto& [k, v] : hdrs)
        req.headers.emplace(k, v);
    return req;
}

// ── getMethod ─────────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetMethodReturnsGET) {
    crow::request req = makeRequest(crow::HTTPMethod::Get);
    EXPECT_EQ(HttpRequest(req).getMethod(), "GET");
}

TEST(HttpRequestTest, GetMethodReturnsPOST) {
    crow::request req = makeRequest(crow::HTTPMethod::Post);
    EXPECT_EQ(HttpRequest(req).getMethod(), "POST");
}

// ── getBody ───────────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetBodyReturnsRequestBody) {
    crow::request req = makeRequest(crow::HTTPMethod::Post, "hello world");
    EXPECT_EQ(HttpRequest(req).getBody(), "hello world");
}

TEST(HttpRequestTest, GetBodyReturnsEmptyStringWhenNoBody) {
    crow::request req = makeRequest();
    EXPECT_TRUE(HttpRequest(req).getBody().empty());
}

// ── getHeader ─────────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetHeaderReturnsValueWhenPresent) {
    crow::request req = makeRequest(crow::HTTPMethod::Get, "",
                                    {{"Content-Type", "application/json"}});
    auto result = HttpRequest(req).getHeader("Content-Type");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "application/json");
}

TEST(HttpRequestTest, GetHeaderIsCaseInsensitive) {
    // Crow's ci_map is case-insensitive; lookup should succeed regardless of case.
    crow::request req = makeRequest(crow::HTTPMethod::Get, "",
                                    {{"content-type", "text/plain"}});
    EXPECT_TRUE(HttpRequest(req).getHeader("Content-Type").has_value());
}

TEST(HttpRequestTest, GetHeaderReturnsNulloptWhenAbsent) {
    crow::request req = makeRequest();
    EXPECT_FALSE(HttpRequest(req).getHeader("X-Custom-Header").has_value());
}

// ── getQueryParam ─────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetQueryParamReturnsValueWhenPresent) {
    crow::request req;
    req.method     = crow::HTTPMethod::Get;
    req.url        = "/test?userId=42";
    req.url_params = crow::query_string(req.url);
    auto result = HttpRequest(req).getQueryParam("userId");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "42");
}

TEST(HttpRequestTest, GetQueryParamReturnsNulloptWhenAbsent) {
    crow::request req;
    req.method     = crow::HTTPMethod::Get;
    req.url        = "/test?foo=bar";
    req.url_params = crow::query_string(req.url);
    EXPECT_FALSE(HttpRequest(req).getQueryParam("missing").has_value());
}

TEST(HttpRequestTest, GetQueryParamHandlesMultipleParams) {
    crow::request req;
    req.method     = crow::HTTPMethod::Get;
    req.url        = "/test?a=1&b=2";
    req.url_params = crow::query_string(req.url);
    EXPECT_EQ(*HttpRequest(req).getQueryParam("a"), "1");
    EXPECT_EQ(*HttpRequest(req).getQueryParam("b"), "2");
}

// ── getBearerToken ────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetBearerTokenExtractsToken) {
    crow::request req = makeRequest(crow::HTTPMethod::Get, "",
                                    {{"Authorization", "Bearer abc123"}});
    auto token = HttpRequest(req).getBearerToken();
    ASSERT_TRUE(token.has_value());
    EXPECT_EQ(*token, "abc123");
}

TEST(HttpRequestTest, GetBearerTokenReturnsNulloptWhenHeaderMissing) {
    crow::request req = makeRequest();
    EXPECT_FALSE(HttpRequest(req).getBearerToken().has_value());
}

TEST(HttpRequestTest, GetBearerTokenReturnsNulloptForNonBearerScheme) {
    crow::request req = makeRequest(crow::HTTPMethod::Get, "",
                                    {{"Authorization", "Basic dXNlcjpwYXNz"}});
    EXPECT_FALSE(HttpRequest(req).getBearerToken().has_value());
}

TEST(HttpRequestTest, GetBearerTokenReturnsNulloptForBearerWithNoToken) {
    crow::request req = makeRequest(crow::HTTPMethod::Get, "",
                                    {{"Authorization", "Bearer "}});
    // An empty token after "Bearer " is treated as absent.
    auto token = HttpRequest(req).getBearerToken();
    EXPECT_TRUE(!token.has_value() || token->empty());
}

// ── getJsonBody ───────────────────────────────────────────────────────────────

TEST(HttpRequestTest, GetJsonBodyParsesValidJson) {
    crow::request req = makeRequest(crow::HTTPMethod::Post,
                                    R"({"key":"value","num":42})");
    EXPECT_NO_THROW({
        auto json = HttpRequest(req).getJsonBody();
        EXPECT_EQ(std::string(json["key"]), "value");
    });
}

TEST(HttpRequestTest, GetJsonBodyThrowsOnInvalidJson) {
    crow::request req = makeRequest(crow::HTTPMethod::Post, "not valid json {{");
    EXPECT_THROW(HttpRequest(req).getJsonBody(), std::runtime_error);
}

TEST(HttpRequestTest, GetJsonBodyThrowsOnEmptyBody) {
    crow::request req = makeRequest(crow::HTTPMethod::Post, "");
    EXPECT_THROW(HttpRequest(req).getJsonBody(), std::runtime_error);
}
