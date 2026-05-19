#pragma once
#include <crow.h>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructureServices/persistance/UserRepository.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"

void registerAllRoutes(crow::SimpleApp& app, OpenAIClient& openai,
                       UserRepository& userRepo, IJwtService& jwt, IPasswordHasher& hasher);
