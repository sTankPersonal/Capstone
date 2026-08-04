#pragma once

#include "ChatMessage.h"
#include "UserProfileDto.h"
#include <string>
#include <vector>

class ILlmClient {
public:
    virtual ~ILlmClient() = default;
    virtual std::string generate(const std::string& systemPrompt,
                                 const std::vector<ChatMessage>& history,
                                 const std::string& userMessage,
                                 const std::optional<UserProfileDto>& userContext) = 0;
};
