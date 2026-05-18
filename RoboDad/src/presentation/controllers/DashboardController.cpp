#include "DashboardController.h"
#include <crow.h>

void DashboardController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/dashboard")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/dashboard.html");
        res.end();
            });
}
