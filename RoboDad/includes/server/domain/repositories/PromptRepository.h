#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include "llm/Prompt.hpp"

class PromptRepository {
public:
    virtual ~PromptRepository() = default;
    virtual Prompt create(uint32_t userId, const Prompt& prompt) = 0;
    virtual std::optional<Prompt> findById(uint32_t id) = 0;
    virtual std::vector<Prompt> findByUserId(uint32_t userId) = 0;
    virtual bool update(const Prompt& prompt) = 0;
    virtual bool remove(uint32_t id) = 0;
};
