#pragma once

#include <crow.h>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"

class AIController {
public:
    explicit AIController(OpenAIClient& client);

    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);

private:
    OpenAIClient& client_;
};
