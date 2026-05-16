#include "Routes.h"
#include "../controllers/ChatController.h"
#include "../controllers/DashboardController.h"
#include "../controllers/SettingsController.h"
#include "../controllers/ActivityController.h"
#include "../controllers/AuthenticationController.h"

void registerAllRoutes(crow::SimpleApp& app, OpenAIClient& openai) {
    // Static assets
    CROW_ROUTE(app, "/assets/<path>")
        ([](const crow::request&, crow::response& res, std::string path) {
        res.set_static_file_info("public/" + path);
        res.end();
            });

    // Controllers
    ChatController(openai).registerRoutes(app);
    DashboardController().registerRoutes(app);
    SettingsController().registerRoutes(app);
    ActivityController().registerRoutes(app);
    AuthenticationController().registerRoutes(app);

    // Root
    CROW_ROUTE(app, "/")([] {
        return "<h1>RoboDad Server Running</h1>";
        });
}