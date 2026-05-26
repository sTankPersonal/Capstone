#include "ExpensesController.h"
#include "../routes/Routes.h"
#include <crow.h>

void ExpensesController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/expenses")
        ([this](const crow::request& req, crow::response& res) {
        if (!requireAuth(req, jwt_)) {
            res.code = 302;
            res.set_header("Location", "/login");
            res.end();
            return;
        }
        res.set_header("Cache-Control", "no-store, no-cache, must-revalidate");
        res.set_header("Pragma", "no-cache");
        res.set_static_file_info("public/expenses.html");
        res.end();
            });
}