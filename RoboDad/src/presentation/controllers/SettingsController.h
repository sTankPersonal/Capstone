#pragma once

#include <crow.h>

class SettingsController {
public:
    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);
};
