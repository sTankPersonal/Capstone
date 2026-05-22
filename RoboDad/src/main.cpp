#include <crow.h>
#include "presentation/routes/Routes.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "infrastructure/config/AppConfig.h"
#include "infrastructure/persistence/DatabaseConnection.h"
#include "infrastructure/persistence/postgres/PostgresUserRepository.h"
#include "infrastructure/security/JwtService.h"
#include "infrastructure/security/PasswordHasher.h"

int main() {
    crow::SimpleApp app;

    auto config = AppConfig::fromEnv();
    OpenAIClient openai(config.openAiApiKey(), config.openAiModel());

    auto db = DatabaseConnection::fromEnv();
    PostgresUserRepository userRepo(db);

    JwtService jwt(config.jwtSecret());
    PasswordHasher hasher;

    registerAllRoutes(app, openai, userRepo, jwt, hasher, config);

    app.bindaddr("0.0.0.0").port(18080).multithreaded().run();
}