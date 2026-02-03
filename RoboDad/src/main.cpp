#include <crow.h>
#include "presentation/routes/Routes.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "infrastructure/apiClient/plaidClient/plaidClient.h"
#include "infrastructure/config/AppConfig.h"
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "infrastructure/persistence/postgres/PostgresChatSessionRepository.h"
#include "infrastructure/persistence/postgres/PostgresChatMessageRepository.h"
#include "infrastructure/persistence/postgres/PostgresLlmPersonaRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionRepository.h"
#include "infrastructure/persistence/postgres/PostgresTransactionCategoryRepository.h"
#include "infrastructure/persistence/postgres/PostgresCurrencyRepository.h"
#include "infrastructure/security/JwtService.h"
#include "infrastructure/security/PasswordHasher.h"

int main() {
    crow::SimpleApp app;

    AppConfig config = AppConfig::fromEnv();
    auto db     = DatabaseConnection::fromEnv();

    OpenAIClient  openai(config.openAiApiKey(), config.openAiModel());
    PromptBuilder promptBuilder;
    PlaidClient   plaid(config.plaidClientId(), config.plaidSecret());

    PostgresUserRepository                userRepo(db);
    PostgresChatSessionRepository         chatSessionRepo(db);
    PostgresChatMessageRepository         chatMessageRepo(db);
    PostgresLlmPersonaRepository          personaRepo(db);
    PostgresTransactionRepository         transactionRepo(db);
    PostgresTransactionCategoryRepository categoryRepo(db);
    PostgresCurrencyRepository            currencyRepo(db);

    JwtService    jwt(config.jwtSecret());
    PasswordHasher hasher;

    registerAllRoutes(app,
                      openai, promptBuilder, plaid,
                      userRepo,
                      chatSessionRepo, chatMessageRepo, personaRepo,
                      transactionRepo, categoryRepo, currencyRepo,
                      jwt, hasher, config);

    app.bindaddr("0.0.0.0").port(18080).multithreaded().run();
}
