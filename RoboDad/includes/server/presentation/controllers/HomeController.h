#pragma once

#include "sharedKernel/presentation/IController.h"
#include "presentation/AppType.h"

class HomeController : public IController<RoboDadApp> {
    std::string cssBasePath_ = "public/css/";
    std::string jsBasePath_ = "public/js/";
    std::string componentsBasePath_ = "public/components/";
public:
    void registerRoutes(RoboDadApp& app) override;
    crow::response getHomePage(const crow::request& req);
    crow::response fetchCSS(const crow::request& req, const std::string& filename);
    crow::response fetchJS(const crow::request& req, const std::string& filename);
    crow::response fetchComponent(const crow::request& req, const std::string& filename);
};
