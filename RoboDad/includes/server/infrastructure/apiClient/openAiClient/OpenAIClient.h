#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "infrastructureServices/apiClient/ILlmClient.h"
#include "llm/Prompt.hpp"
#include <string>

// HTTP client for OpenAI's /v1/chat/completions endpoint.
// Inherits curl transport, API-key storage, and isConfigured() from ApiClient.
class OpenAIClient : public ApiClient, public ILlmClient {
    std::string model_;   // e.g. "gpt-4o-mini"

public:
    // apiKey: OpenAI API key.
    // model:  OpenAI model name.
    OpenAIClient(const std::string& apiKey, const std::string& model);

    // Sends a fully constructed Prompt to OpenAI's chat/completions endpoint
    // and returns the assistant's response text.
    // Throws std::runtime_error on HTTP error, curl failure, or malformed response.
    std::string generate(const Prompt& prompt) override;

    const std::string& apiKey() const { return apiKey_; }

private:
    // Serializes a Prompt into OpenAI's chat/completions JSON body.
    std::string buildRequestPayload(const Prompt& prompt) const;

    // Extracts choices[0].message.content from an OpenAI response.
    std::string parseResponse(const std::string& jsonResponse) const;
};
