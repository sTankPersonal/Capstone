#include "AuthenticationController.h"
#include <crow.h>

void AuthenticationController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/logout")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
            });
    CROW_ROUTE(app, "/login")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
            });
    CROW_ROUTE(app, "/signup")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/signup.html");
        res.end();
            });
}
