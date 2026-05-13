#pragma once
#include "llm/Prompt.hpp"
#include <string>

// HTTP client that calls a external OpenAI REST API to get LLM responses.
// Uses libcurl for the underlying HTTP transport.
class OpenAIClient {
    std::string apiKey_;
    std::string model_;   // e.g. "GPT-4o"

public:
    // apiKey: OpenAI API key.
    // model:  OpenAI model name.
    OpenAIClient(const std::string& apiKey, const std::string& model);

    // Sends a fully constructed Prompt to OpenAI's /v1/chat/completions endpoint and
    // returns the assistant's response text.
    // Throws std::runtime_error on HTTP error, curl failure, or malformed response.
    std::string generate(const Prompt& prompt);

private:
    // Serializes a Prompt domain object into OpenAI's chat/completions JSON body.
    // Maps SystemInstructions to the "system" role, ChatHistory entries to
    // alternating "user"/"assistant" roles, and UserMessage to the final "user" turn.
    std::string buildRequestPayload(const Prompt& prompt) const;

    // Deserializes an OpenAI chat/completions response and extracts
    // choices[0].message.content.
    std::string parseResponse(const std::string& jsonResponse) const;

    // Makes an HTTP POST to OpenAI's chat/completions endpoint.
    // Returns the raw response body string.
    // Throws std::runtime_error if the connection fails or the server returns 4xx/5xx.
    std::string post(const std::string& body) const;
};
