#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "infrastructureServices/apiClient/ILlmClient.h"
#include "ChatMessage.h"
#include <string>
#include <vector>

class OpenAIClient : public ApiClient, public ILlmClient {
    std::string model_;

public:
    OpenAIClient(const std::string& apiKey, const std::string& model);

    std::string generate(const std::string&              systemPrompt,
                         const std::vector<ChatMessage>& history,
                         const std::string&              userMessage) override;

    const std::string& apiKey() const { return apiKey_; }

private:
    std::string buildRequestPayload(const std::string&              systemPrompt,
                                    const std::vector<ChatMessage>& history,
                                    const std::string&              userMessage) const;
    std::string parseResponse(const std::string& jsonResponse) const;
};
