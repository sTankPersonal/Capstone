#pragma once

#include <crow.h>

class DashboardController {
public:
    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);
};
