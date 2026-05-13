#pragma once
#include <crow.h>
#include <string>
#include <vector>
#include <utility>

// Builds an HTTP response to be returned from a Crow route handler.
// Use the static factory helpers for common status codes.
class HttpResponse {
    int statusCode_ = 200;
    std::string body_;
    std::vector<std::pair<std::string, std::string>> headers_;

public:
    explicit HttpResponse(int statusCode = 200);

    // Sets the HTTP status code.
    HttpResponse& setStatus(int code);

    // Serializes the given JSON value and sets it as the response body.
    // Also sets Content-Type: application/json.
    HttpResponse& setBody(const crow::json::wvalue& json);

    // Adds a custom header key-value pair to the response.
    HttpResponse& setHeader(const std::string& key, const std::string& value);

    // Converts this HttpResponse into a crow::response ready for Crow to send.
    crow::response toCrowResponse() const;

    // ---- Static factory helpers ----

    // 200 OK with the given JSON payload.
    static HttpResponse ok(const crow::json::wvalue& body);

    // 201 Created with the given JSON payload.
    static HttpResponse created(const crow::json::wvalue& body);

    // 400 Bad Request with {"error": message}.
    static HttpResponse badRequest(const std::string& message);

    // 401 Unauthorized with {"error": message}.
    static HttpResponse unauthorized(const std::string& message);

    // 404 Not Found with {"error": message}.
    static HttpResponse notFound(const std::string& message);

    // 500 Internal Server Error with {"error": message}.
    static HttpResponse internalError(const std::string& message);
};
