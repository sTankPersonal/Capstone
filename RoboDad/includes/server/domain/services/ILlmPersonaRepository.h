#pragma once
#include "IRepository.h"
#include "LlmPersona.h"
#include "LlmPersonaId.h"

class ILlmPersonaRepository : public IRepository<LlmPersona, LlmPersonaId> {};
