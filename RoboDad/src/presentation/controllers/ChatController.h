#pragma once

#include <crow.h>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructureServices/security/IJwtService.h"

class ChatController {
public:
    explicit ChatController(OpenAIClient& client, IJwtService& jwt);
    void registerRoutes(crow::SimpleApp& app);

private:
    OpenAIClient& client_;
    IJwtService& jwt_;
};