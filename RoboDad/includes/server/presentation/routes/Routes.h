#pragma once
#include <crow.h>
#include "IUserRepository.h"
#include "IChatSessionRepository.h"
#include "IChatMessageRepository.h"
#include "ILlmPersonaRepository.h"
#include "ITransactionRepository.h"
#include "ITransactionCategoryRepository.h"
#include "ICurrencyRepository.h"
#include "infrastructureServices/apiClient/ILlmClient.h"
#include "infrastructureServices/apiClient/IPromptBuilder.h"
#include "infrastructureServices/apiClient/IPlaidClient.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"
#include "infrastructure/config/AppConfig.h"

void registerAllRoutes(crow::SimpleApp&         app,
                       ILlmClient&              llmClient,
                       IPromptBuilder&          promptBuilder,
                       IPlaidClient&            plaidClient,
                       IUserRepository&         userRepo,
                       IChatSessionRepository&  chatSessionRepo,
                       IChatMessageRepository&  chatMessageRepo,
                       ILlmPersonaRepository&   personaRepo,
                       ITransactionRepository&         transactionRepo,
                       ITransactionCategoryRepository& categoryRepo,
                       ICurrencyRepository&            currencyRepo,
                       IJwtService&             jwt,
                       IPasswordHasher&         hasher,
                       const AppConfig&        config);
