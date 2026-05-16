#include "ChatController.h"
#include "infrastructure/apiClient/openAiClient/PromptBuilder.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"
#include <crow.h>

ChatController::ChatController(OpenAIClient& client)
    : client_(client) {}

void ChatController::registerRoutes(crow::SimpleApp& app) {

    CROW_ROUTE(app, "/chat")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/chat.html");
        res.end();
            });


    CROW_ROUTE(app, "/api/ai/chat").methods("POST"_method)
    ([this](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body || !body.has("message")) {
            return crow::response(400, "Missing 'message'");
        }

        std::string userMessage = body["message"].s();

        // Build a prompt using your new PromptBuilder
        PromptBuilder builder;
        Prompt prompt = builder
            .withUserMessage(userMessage)
            .build();  // uses default RoboDad system instructions

        try {
            // If no API key is set, return a stub response instead of calling OpenAI
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
