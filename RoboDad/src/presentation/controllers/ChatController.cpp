#include "ChatController.h"
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include "../routes/Routes.h"
#include <crow.h>

ChatController::ChatController(OpenAIClient& client, IJwtService& jwt)
    : client_(client), jwt_(jwt) {
}

void ChatController::registerRoutes(crow::SimpleApp& app) {

    CROW_ROUTE(app, "/chat")
        ([this](const crow::request& req, crow::response& res) {
        if (!requireAuth(req, jwt_)) {
            res.code = 302;
            res.set_header("Location", "/login");
            res.end();
            return;
        }
        res.set_header("Cache-Control", "no-store, no-cache, must-revalidate");
        res.set_header("Pragma", "no-cache");
        res.set_static_file_info("public/chat.html");
        res.end();
            });

    CROW_ROUTE(app, "/api/ai/chat").methods("POST"_method)
        ([this](const crow::request& req) {
        if (!requireAuth(req, jwt_)) {
            return crow::response(401, R"({"error":"Unauthorized"})");
        }

        auto body = crow::json::load(req.body);
        if (!body || !body.has("message")) {
            return crow::response(400, "Missing 'message'");
        }

        std::string userMessage = body["message"].s();

        PromptBuilder builder;
        Prompt prompt = builder
            .withUserMessage(userMessage)
            .build();

        try {
            if (client_.apiKey().empty()) {
                crow::json::wvalue out;
                out["reply"] = "AI disabled (no API key). This is a stub response.";
                return crow::response(200, out.dump());
            }

            std::string reply = client_.generate(prompt);

            crow::json::wvalue out;
            out["reply"] = reply;
            return crow::response(200, out.dump());
        }
        catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
            });
}