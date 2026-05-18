#include "ActivityController.h"
#include <crow.h>

void ActivityController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/activity")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/activity.html");
        res.end();
            });
}
