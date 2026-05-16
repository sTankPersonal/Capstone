#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include <stdexcept>
#include <iostream> // For debugging; remove in production
#include <crow.h>

OpenAIClient::OpenAIClient(const std::string& apiKey, const std::string& model)
    : ApiClient(apiKey, "https://api.openai.com"), model_(model) {}

std::string OpenAIClient::generate(const Prompt& prompt) {
    std::cout << "generate" << std::endl;
    std::cout << "generate content Message: " << prompt.getUserMessage() << std::endl;

    const std::string payload = buildRequestPayload(prompt);
    const std::string raw = httpPost(
        baseUrl_ + "/v1/chat/completions",
        payload,
        {
            "Content-Type: application/json",
            "Authorization: Bearer " + apiKey_
        }
    );
    return parseResponse(raw);
}

std::string OpenAIClient::buildRequestPayload(const Prompt& prompt) const {
    std::cout << "buildRequestPayload" << std::endl;
    crow::json::wvalue root;
    root["model"] = model_;
    root["stream"] = false;

    crow::json::wvalue messages = crow::json::wvalue::list();
    int index = 0;

    // ---- System message ----
    std::string sysContent =
        prompt.getSystemInstructions().getPersonality() + "\n" +
        prompt.getSystemInstructions().getSafetyGuidelines() + "\n" +
        prompt.getSystemInstructions().getGoal();

    messages[index++] = crow::json::wvalue{
        {"role", "system"},
        {"content", sysContent}
    };

    // ---- Chat history ----
    for (const auto& entry : prompt.getChatHistories()) {
        messages[index++] = crow::json::wvalue{
            {"role", "user"},
            {"content", entry.getUserMessage()}
        };
        messages[index++] = crow::json::wvalue{
            {"role", "assistant"},
            {"content", entry.getLLMResponse()}
        };
    }

    // ---- User message ----
    messages[index++] = crow::json::wvalue{
        {"role", "user"},
        {"content", prompt.getUserMessage()}
    };

    root["messages"] = std::move(messages);
    return root.dump();
}

std::string OpenAIClient::parseResponse(const std::string& jsonResponse) const {
    std::cout << "parseResponse" << std::endl;

    auto json = crow::json::load(jsonResponse);
    if (!json) {
        throw std::runtime_error("OpenAIClient: invalid JSON response");
    }

    if (!json.has("choices") || json["choices"].size() == 0) {
        throw std::runtime_error("OpenAIClient: missing 'choices' in response");
    }

    auto& choice0 = json["choices"][0];
    if (!choice0.has("message") || !choice0["message"].has("content")) {
        throw std::runtime_error("OpenAIClient: missing assistant message content");
    }

    return choice0["message"]["content"].s();
}
