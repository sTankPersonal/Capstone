#include "Routes.h"
#include <sstream>
#include "../controllers/ChatController.h"
#include "../controllers/DashboardController.h"
#include "../controllers/SettingsController.h"
#include "../controllers/ActivityController.h"
#include "../controllers/AuthenticationController.h"
#include "../controllers/GoogleAuthController.h"

std::optional<uint32_t> requireAuth(const crow::request& req, IJwtService& jwt) {
    std::string cookieHeader = req.get_header_value("Cookie");
    if (cookieHeader.empty()) return std::nullopt;

    std::string token;
    std::istringstream stream(cookieHeader);
    std::string pair;
    while (std::getline(stream, pair, ';')) {
        size_t start = pair.find_first_not_of(' ');
        if (start == std::string::npos) continue;
        pair = pair.substr(start);
        if (pair.rfind("token=", 0) == 0) {
            token = pair.substr(6);
            break;
        }
    }

    if (token.empty()) return std::nullopt;
    return jwt.verify(token);
}

void registerAllRoutes(crow::SimpleApp& app, OpenAIClient& openai,
    UserRepository& userRepo, IJwtService& jwt,
    IPasswordHasher& hasher, const AppConfig& config) {
    CROW_ROUTE(app, "/assets/<path>")
        ([](const crow::request&, crow::response& res, std::string path) {
        res.set_static_file_info("public/" + path);
        res.end();
            });

    (new ChatController(openai, jwt))->registerRoutes(app);
    (new DashboardController(jwt))->registerRoutes(app);
    (new SettingsController(jwt))->registerRoutes(app);
    (new ActivityController(jwt))->registerRoutes(app);
    (new AuthenticationController(userRepo, hasher, jwt))->registerRoutes(app);
    (new GoogleAuthController(userRepo, jwt, hasher,
        config.googleClientId(),
        config.googleClientSecret()))->registerRoutes(app);

    CROW_ROUTE(app, "/api/user/me")
        ([&jwt, &userRepo](const crow::request& req) {
        auto userId = requireAuth(req, jwt);
        if (!userId) return crow::response(401, R"({"error":"Unauthorized"})");
        auto user = userRepo.findById(*userId);
        if (!user) return crow::response(404, R"({"error":"User not found"})");
        crow::json::wvalue out;
        out["name"] = user->getPersonalInfo().getName();
        return crow::response(200, out.dump());
            });

    CROW_ROUTE(app, "/")([] {
        return "<h1>RoboDad Server Running</h1>";
        });
}