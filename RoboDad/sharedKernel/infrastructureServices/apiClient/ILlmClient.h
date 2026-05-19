#pragma once
#include <string>
#include "domain/llm/Prompt.h"

class ILlmClient {
public:
    virtual ~ILlmClient() = default;
    virtual std::string generate(const Prompt& prompt) = 0;
};
