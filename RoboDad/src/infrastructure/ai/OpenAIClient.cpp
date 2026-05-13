#include "infrastructure/ai/OpenAIClient.h"
#include <stdexcept>

OpenAIClient::OpenAIClient(const std::string& apiKey, const std::string& model)
    : apiKey_(apiKey), model_(model) {}

std::string OpenAIClient::generate(const Prompt& prompt) {
    // TODO: call buildRequestPayload(prompt) to get the JSON body string;
    //       call post(body) to get the raw response string;
    //       call parseResponse(rawResponse) and return the assistant text
}

std::string OpenAIClient::buildRequestPayload(const Prompt& prompt) const {
    // TODO: build a JSON object with "model" = model_ and a "messages" array:
    //         - first element: {"role":"system", "content": prompt.getSystemInstructions().text()}
    //         - for each ChatHistory entry: push {"role":"user", "content": entry.getUserMessage()}
    //                                       then {"role":"assistant", "content": entry.getLLMResponse()}
    //         - last element: {"role":"user", "content": prompt.getUserMessage()}
    //       set "stream": false to get a single response object;
    //       serialize to a JSON string and return it
}

std::string OpenAIClient::parseResponse(const std::string& jsonResponse) const {
    // TODO: parse jsonResponse as JSON;
    //       navigate to choices[0].message.content and return that string;
    //       throw std::runtime_error if the expected fields are absent or the JSON is malformed
}

std::string OpenAIClient::post(const std::string& body) const {
    // TODO: initialize a CURL handle with curl_easy_init();
    //       set CURLOPT_URL to "https://api.openai.com/v1/chat/completions";
    //       set CURLOPT_POSTFIELDS to body.c_str();
    //       set Content-Type: application/json and Authorization: Bearer <apiKey_>;
    //       use a write callback to accumulate the response into a std::string;
    //       call curl_easy_perform(); on failure throw std::runtime_error with the curl error;
    //       check the HTTP response code; if >= 400 throw std::runtime_error with status + body;
    //       clean up with curl_easy_cleanup() and return the accumulated response string
}
