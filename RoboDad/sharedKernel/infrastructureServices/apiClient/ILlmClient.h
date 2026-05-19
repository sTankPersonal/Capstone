#pragma once
#include <string>
#include "llm/Prompt.hpp"

class ILlmClient {
public:
    virtual ~ILlmClient() = default;
    virtual std::string generate(const Prompt& prompt) = 0;
};
