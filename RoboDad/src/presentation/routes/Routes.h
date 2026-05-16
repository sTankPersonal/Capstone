#pragma once
#include <crow.h>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"

void registerAllRoutes(crow::SimpleApp& app, OpenAIClient& openai);
