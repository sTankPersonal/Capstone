#pragma once
#include "ChatMessage.h"
#include <string>
#include <vector>

class ILlmClient {
public:
    virtual ~ILlmClient() = default;

    // systemPrompt: the persona's system prompt extracted by the use case.
    // history: prior ChatMessages for this session, oldest first.
    virtual std::string generate(const std::string& systemPrompt,
                                 const std::vector<ChatMessage>& history,
                                 const std::string& userMessage) = 0;
};
