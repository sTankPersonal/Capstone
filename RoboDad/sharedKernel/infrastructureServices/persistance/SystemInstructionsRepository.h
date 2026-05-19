#pragma once
#include "IRepository.h"
#include "llm/SystemInstructions.hpp"

class SystemInstructionsRepository : public IRepository<SystemInstructions> {
public:
    virtual SystemInstructions create(const SystemInstructions& si) = 0;
};
