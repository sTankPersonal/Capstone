#pragma once

#include <crow.h>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"

class ChatController {
public:
    explicit ChatController(OpenAIClient& client);

    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);

private:
    OpenAIClient& client_;
};
