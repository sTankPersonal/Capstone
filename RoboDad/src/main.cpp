#include <crow.h>
#include "presentation/routes/Routes.h"
#include "infrastructure/apiClient/openAiClient/OpenAIClient.h"

int main() {
    crow::SimpleApp app;

    const char* key = std::getenv("OPENAI_API_KEY");
    OpenAIClient openai(key ? key : "", "gpt-4o-mini");

    registerAllRoutes(app, openai);

    app.bindaddr("0.0.0.0").port(18080).multithreaded().run();
}