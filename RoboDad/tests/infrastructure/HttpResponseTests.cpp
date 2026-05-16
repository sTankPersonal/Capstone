#include <gtest/gtest.h>
#include <crow.h>
#include "infrastructure/http/HttpResponse.h"

// Helper: parse a crow::response body as JSON; returns invalid rvalue on failure.
static crow::json::rvalue parseBody(const crow::response& res) {
    return crow::json::load(res.body);
}

// ── Status codes via static factories ─────────────────────────────────────────

TEST(HttpResponseTest, OkSets200StatusCode) {
    crow::json::wvalue body;
    body["msg"] = "success";
    EXPECT_EQ(HttpResponse::ok(body).toCrowResponse().code, 200);
}

TEST(HttpResponseTest, CreatedSets201StatusCode) {
    crow::json::wvalue body;
    body["id"] = 1;
    EXPECT_EQ(HttpResponse::created(body).toCrowResponse().code, 201);
}

TEST(HttpResponseTest, BadRequestSets400) {
    EXPECT_EQ(HttpResponse::badRequest("bad").toCrowResponse().code, 400);
}

TEST(HttpResponseTest, UnauthorizedSets401) {
    EXPECT_EQ(HttpResponse::unauthorized("denied").toCrowResponse().code, 401);
}

TEST(HttpResponseTest, NotFoundSets404) {
    EXPECT_EQ(HttpResponse::notFound("missing").toCrowResponse().code, 404);
}

TEST(HttpResponseTest, InternalErrorSets500) {
    EXPECT_EQ(HttpResponse::internalError("boom").toCrowResponse().code, 500);
}

// ── Error body shape ──────────────────────────────────────────────────────────

TEST(HttpResponseTest, BadRequestBodyContainsErrorField) {
    auto res = HttpResponse::badRequest("invalid input").toCrowResponse();
    auto json = parseBody(res);
    ASSERT_TRUE(json);
    EXPECT_EQ(std::string(json["error"]), "invalid input");
}

TEST(HttpResponseTest, UnauthorizedBodyContainsErrorField) {
    auto res = HttpResponse::unauthorized("token expired").toCrowResponse();
    auto json = parseBody(res);
    ASSERT_TRUE(json);
    EXPECT_EQ(std::string(json["error"]), "token expired");
}

TEST(HttpResponseTest, NotFoundBodyContainsErrorField) {
    auto res = HttpResponse::notFound("user not found").toCrowResponse();
    auto json = parseBody(res);
    ASSERT_TRUE(json);
    EXPECT_EQ(std::string(json["error"]), "user not found");
}

TEST(HttpResponseTest, InternalErrorBodyContainsErrorField) {
    auto res = HttpResponse::internalError("unexpected failure").toCrowResponse();
    auto json = parseBody(res);
    ASSERT_TRUE(json);
    EXPECT_EQ(std::string(json["error"]), "unexpected failure");
}

// ── ok / created payload passthrough ─────────────────────────────────────────

TEST(HttpResponseTest, OkBodyPreservesJsonPayload) {
    crow::json::wvalue body;
    body["userId"] = 7;
    body["name"]   = "Alice";
    auto res    = HttpResponse::ok(body).toCrowResponse();
    auto parsed = parseBody(res);
    ASSERT_TRUE(parsed);
    EXPECT_EQ((int)parsed["userId"], 7);
    EXPECT_EQ(std::string(parsed["name"]), "Alice");
}

TEST(HttpResponseTest, CreatedBodyPreservesJsonPayload) {
    crow::json::wvalue body;
    body["id"]          = 42;
    body["description"] = "Emergency Fund";
    auto res    = HttpResponse::created(body).toCrowResponse();
    auto parsed = parseBody(res);
    ASSERT_TRUE(parsed);
    EXPECT_EQ((int)parsed["id"], 42);
}

// ── setStatus ─────────────────────────────────────────────────────────────────

TEST(HttpResponseTest, SetStatusOverridesDefault) {
    HttpResponse r;
    EXPECT_EQ(r.setStatus(418).toCrowResponse().code, 418);
}

// ── setHeader ─────────────────────────────────────────────────────────────────

TEST(HttpResponseTest, SetHeaderAppearsInCrowResponse) {
    crow::json::wvalue body;
    body["ok"] = true;
    HttpResponse r;
    r.setBody(body).setHeader("X-Request-Id", "test-id-123");
    auto res = r.toCrowResponse();
    EXPECT_EQ(res.get_header_value("X-Request-Id"), "test-id-123");
}

TEST(HttpResponseTest, MultipleHeadersAllPresent) {
    HttpResponse r;
    r.setHeader("X-A", "alpha").setHeader("X-B", "beta");
    auto res = r.toCrowResponse();
    EXPECT_EQ(res.get_header_value("X-A"), "alpha");
    EXPECT_EQ(res.get_header_value("X-B"), "beta");
}

// ── Content-Type ──────────────────────────────────────────────────────────────

TEST(HttpResponseTest, SetBodyAddsJsonContentTypeHeader) {
    crow::json::wvalue body;
    body["x"] = 1;
    HttpResponse r;
    r.setBody(body);
    auto res = r.toCrowResponse();
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
}
