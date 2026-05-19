#pragma once
#include <cstdint>
#include <vector>
#include "IRepository.h"
#include "llm/Prompt.hpp"

class PromptRepository : public IRepository<Prompt> {
public:
    virtual Prompt create(uint32_t userId, const Prompt& prompt) = 0;
    virtual std::vector<Prompt> findByUserId(uint32_t userId) = 0;
};
