#include <iostream>
#include "crow.h"

#include "presentation/controllers/AiController.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"

int main() {
    crow::SimpleApp app;

    // Basic test route
    CROW_ROUTE(app, "/")([]() {
        return "<h1>Hello, Crow + ASIO is working!</h1>";
    });

    // Load API key
    const char* key = std::getenv("OPENAI_API_KEY");
    if (!key) {
        std::cout << "Warning: OPENAI_API_KEY not set. AI endpoints will return a stub response.\n";
    }
    OpenAIClient openai(key ? key : "", "gpt-4o-mini");

    // Register AI routes
    AIController ai(openai);
    ai.registerRoutes(app);

    std::cout << "Server running on http://127.0.0.1:18080\n";

    app.bindaddr("0.0.0.0").port(18080).multithreaded().run();
}
