#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include <stdexcept>
#include <sstream>
#include <crow.h>

OpenAIClient::OpenAIClient(const std::string& apiKey, const std::string& model)
    : ApiClient(apiKey, "https://api.openai.com"), model_(model) {}

std::string OpenAIClient::generate(const std::string&              systemPrompt,
                                   const std::vector<ChatMessage>& history,
                                   const std::string&              userMessage,
                                   const std::optional<UserProfileDto>& userContext
) {
    const std::string payload = buildRequestPayload(systemPrompt, history, userMessage, userContext);
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

std::string OpenAIClient::buildRequestPayload(
    const std::string& systemPrompt,
    const std::vector<ChatMessage>& history,
    const std::string& userMessage,
    const std::optional<UserProfileDto>& userContext) const
{
    crow::json::wvalue root;
    root["model"] = model_;
    root["stream"] = false;

    crow::json::wvalue messages = crow::json::wvalue::list();
    int index = 0;

    // System prompt
    messages[index++] = crow::json::wvalue{
        {"role", "system"},
        {"content", systemPrompt}
    };

    // Inject user settings as a system-level metadata block
    if (userContext) {
        crow::json::wvalue ctx;
        ctx["firstName"] = userContext->getFirstName().value_or("");
        ctx["lastName"] = userContext->getLastName().value_or("");
        ctx["countryId"] = userContext->getCountryId().value_or("");
        ctx["currencyId"] = userContext->getCurrencyId().value_or("");
        ctx["languageId"] = userContext->getLanguageId().value_or("");
        ctx["employmentStatusId"] = userContext->getEmploymentStatusId().value_or("");

        std::stringstream ss;
        auto firstName = ctx["firstName"].dump();
        auto lastName = ctx["lastName"].dump();
        auto countryId = ctx["countryId"].dump();
        auto currencyId = ctx["currencyId"].dump();
        auto languageId = ctx["languageId"].dump();
        auto employmentStatusId = ctx["employmentStatusId"].dump();

        auto strip = [](std::string& s) {
            if (!s.empty() && s.front() == '"' && s.back() == '"') {
                s = s.substr(1, s.size() - 2);
            }
            };

        strip(firstName);
        strip(lastName);
        strip(countryId);
        strip(currencyId);
        strip(languageId);
        strip(employmentStatusId);

        ss << "First name: " << firstName << "\n";
        ss << "Last name: " << lastName << "\n";
        ss << "Country: " << countryId << "\n";
        ss << "Currency: " << currencyId << "\n";
        ss << "Language: " << languageId << "\n";
        ss << "Employment status: " << employmentStatusId << "\n";

        messages[index++] = crow::json::wvalue{
            {"role", "system"},
            {"content", ss.str()}
        };
    }

    // History
    for (const auto& msg : history) {
        messages[index++] = crow::json::wvalue{
            {"role", msg.getMessageSenderId().getId()},
            {"content", msg.getContent().getContent().value_or("")}
        };
    }

    // User message
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
