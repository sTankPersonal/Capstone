#pragma once

#include <crow.h>

class ActivityController {
public:
    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);
};
