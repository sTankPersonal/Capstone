#pragma once
#include <crow.h>
#include <optional>
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructureServices/persistance/UserRepository.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"
#include "infrastructure/config/AppConfig.h"

void registerAllRoutes(crow::SimpleApp& app, OpenAIClient& openai,
    UserRepository& userRepo, IJwtService& jwt,
    IPasswordHasher& hasher, const AppConfig& config);

std::optional<uint32_t> requireAuth(const crow::request& req, IJwtService& jwt);