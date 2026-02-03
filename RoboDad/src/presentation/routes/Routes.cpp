#include "presentation/routes/Routes.h"
#include "presentation/controllers/ChatController.h"
#include "presentation/controllers/DashboardController.h"
#include "presentation/controllers/SettingsController.h"
#include "presentation/controllers/ActivityController.h"
#include "presentation/controllers/AuthenticationController.h"
#include "presentation/controllers/GoogleAuthController.h"

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
                       const AppConfig&        config) {

    CROW_ROUTE(app, "/assets/<path>")
    ([](const crow::request&, crow::response& res, std::string path) {
        res.set_static_file_info("public/" + path);
        res.end();
    });

    (new AuthenticationController(userRepo, hasher, jwt))->registerRoutes(app);
    (new GoogleAuthController(userRepo, jwt, hasher, config.googleClientId(), config.googleClientSecret()))->registerRoutes(app);
    (new ChatController(chatSessionRepo, chatMessageRepo, personaRepo, transactionRepo, llmClient, promptBuilder, jwt))->registerRoutes(app);
    (new SettingsController(userRepo, personaRepo, hasher, jwt))->registerRoutes(app);
    (new ActivityController(transactionRepo, categoryRepo, currencyRepo, plaidClient, jwt))->registerRoutes(app);
    DashboardController().registerRoutes(app);

    CROW_ROUTE(app, "/")([] {
        return "<h1>RoboDad Server Running</h1>";
    });
}
