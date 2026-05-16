#include "SettingsController.h"
#include <crow.h>

void SettingsController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/settings")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/settings.html");
        res.end();
            });
}
