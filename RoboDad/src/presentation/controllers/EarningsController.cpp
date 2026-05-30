#include "EarningsController.h"
#include "../routes/Routes.h"
#include <crow.h>

void EarningsController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/earnings")
        ([this](const crow::request& req, crow::response& res) {
        if (!requireAuth(req, jwt_)) {
            res.code = 302;
            res.set_header("Location", "/login");
            res.end();
            return;
        }
        res.set_header("Cache-Control", "no-store, no-cache, must-revalidate");
        res.set_header("Pragma", "no-cache");
        res.set_static_file_info("public/earnings.html");
        res.end();
            });
}