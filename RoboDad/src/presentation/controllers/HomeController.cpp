#include "presentation/controllers/HomeController.h"

void HomeController::registerRoutes(RoboDadApp& app) {
    CROW_ROUTE(app, "/").methods(crow::HTTPMethod::GET)([this](const crow::request& req) {
        return getHomePage(req);
    });
    CROW_ROUTE(app, "/css/<path>").methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& filename) {
        return fetchCSS(req, filename);
    });
    CROW_ROUTE(app, "/js/<path>").methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& filename) {
        return fetchJS(req, filename);
    });
    CROW_ROUTE(app, "/components/<path>").methods(crow::HTTPMethod::GET)([this](const crow::request& req, const std::string& filename) {
        return fetchComponent(req, filename);
    });
}

crow::response HomeController::getHomePage(const crow::request& req) {
    crow::response res;
    res.redirect("/auth/login");
    return res;
}

crow::response HomeController::fetchCSS(const crow::request& req, const std::string& filename) {
    crow::response res;
    res.set_static_file_info(cssBasePath_ + filename);
    return res;
}

crow::response HomeController::fetchJS(const crow::request& req, const std::string& filename) {
    crow::response res;
    res.set_static_file_info(jsBasePath_ + filename);
    return res;
}

crow::response HomeController::fetchComponent(const crow::request& req, const std::string& filename) {
    crow::response res;
    res.set_static_file_info(componentsBasePath_ + filename);
    return res;
}
