#pragma once
#include <cstdint>
#include <optional>
#include "llm/SystemInstructions.hpp"

class SystemInstructionsRepository {
public:
    virtual ~SystemInstructionsRepository() = default;
    virtual SystemInstructions create(const SystemInstructions& si) = 0;
    virtual std::optional<SystemInstructions> findById(uint32_t id) = 0;
    virtual bool update(const SystemInstructions& si) = 0;
    virtual bool remove(uint32_t id) = 0;
};
