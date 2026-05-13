#pragma once
#include <crow.h>
#include <optional>
#include <string>

// Wraps a crow::request to give route handlers a clean, Crow-agnostic interface.
class HttpRequest {
    const crow::request& req_;

public:
    explicit HttpRequest(const crow::request& req);

    // Returns the HTTP method as an uppercase string (e.g. "GET", "POST").
    std::string getMethod() const;

    // Returns the raw request body as a string.
    std::string getBody() const;

    // Returns the value of the named header, or nullopt if the header is absent.
    std::optional<std::string> getHeader(const std::string& name) const;

    // Returns the value of a URL query parameter (e.g. ?key=val), or nullopt if absent.
    std::optional<std::string> getQueryParam(const std::string& key) const;

    // Extracts the Bearer token from "Authorization: Bearer <token>",
    // returning nullopt if the header is missing or the scheme is not Bearer.
    std::optional<std::string> getBearerToken() const;

    // Parses the body as JSON and returns the result.
    // Throws std::runtime_error if the body is not valid JSON.
    crow::json::rvalue getJsonBody() const;
};
