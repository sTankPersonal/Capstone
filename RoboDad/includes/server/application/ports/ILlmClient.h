#pragma once

#include "ChatMessage.h"
#include <string>
#include <vector>

class ILlmClient {
public:
    virtual ~ILlmClient() = default;
    virtual std::string generate(const std::string& systemPrompt,
                                 const std::vector<ChatMessage>& history,
                                 const std::string& userMessage) = 0;
};
