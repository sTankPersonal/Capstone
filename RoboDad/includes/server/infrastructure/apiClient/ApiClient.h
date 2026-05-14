#pragma once
#include <string>
#include <vector>

// Base class for all external HTTP API clients.
// Holds common config (API key, base URL) and provides authenticated
// httpPost / httpGet helpers built on libcurl, so concrete clients like
// OpenAIClient or PlaidClient only need to handle their own endpoints,
// request serialization, and response parsing.
class ApiClient {
protected:
    std::string apiKey_;
    std::string baseUrl_;

    ApiClient(std::string apiKey, std::string baseUrl);

    // Performs an HTTP POST to `url` with `body` as the request payload.
    // `headers` is a list of raw "Key: Value" strings, e.g.:
    //   {"Content-Type: application/json", "Authorization: Bearer sk-..."}
    // Returns the raw response body on success.
    // Throws std::runtime_error on curl failure or HTTP 4xx/5xx.
    std::string httpPost(const std::string& url, const std::string& body,
                         const std::vector<std::string>& headers) const;

    // Performs an HTTP GET to `url`.
    // `headers` follows the same format as httpPost.
    // Returns the raw response body on success.
    // Throws std::runtime_error on curl failure or HTTP 4xx/5xx.
    std::string httpGet(const std::string& url,
                        const std::vector<std::string>& headers) const;

public:
    virtual ~ApiClient() = default;

    // Returns true if the client has the credentials needed to make real API calls.
    bool isConfigured() const { return !apiKey_.empty(); }
};
