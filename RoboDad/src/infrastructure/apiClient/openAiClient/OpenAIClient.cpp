#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include <stdexcept>
#include <sstream>
#include <crow.h>

OpenAIClient::OpenAIClient(const std::string& apiKey, const std::string& model)
    : ApiClient(apiKey, "https://api.openai.com"), model_(model) {}

std::string OpenAIClient::generate(const std::string&              systemPrompt,
                                   const std::vector<ChatMessage>& history,
                                   const std::string&              userMessage) {
    const std::string payload = buildRequestPayload(systemPrompt, history, userMessage);
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

std::string OpenAIClient::buildRequestPayload(const std::string&              systemPrompt,
                                              const std::vector<ChatMessage>& history,
                                              const std::string&              userMessage) const {
    crow::json::wvalue root;
    root["model"] = model_;
    root["stream"] = false;

    crow::json::wvalue messages = crow::json::wvalue::list();
    int index = 0;

    if (!systemPrompt.empty()) {
        messages[index++] = crow::json::wvalue{
            {"role", "system"},
            {"content", systemPrompt}
        };
    }

    for (const auto& msg : history) {
        // MessageSenderId value is "user" or "assistant" — matches OpenAI role names.
        const std::string role = msg.getMessageSenderId().getId();
        const std::string content = msg.getContent().getContent().value_or(std::string{});
        messages[index++] = crow::json::wvalue{
            {"role", role},
            {"content", content}
        };
    }

    messages[index++] = crow::json::wvalue{
        {"role", "user"},
        {"content", userMessage}
    };

    root["messages"] = std::move(messages);
    return root.dump();
}

std::string OpenAIClient::parseResponse(const std::string& jsonResponse) const {
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
