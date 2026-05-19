#pragma once
#include "IRepository.h"
#include "domain/llm/SystemInstructions.h"

class SystemInstructionsRepository : public IRepository<SystemInstructions> {
public:
    virtual SystemInstructions create(const SystemInstructions& si) = 0;
};
