#pragma once

#include <crow.h>

class AuthenticationController {
public:
    // Registers all AI-related HTTP routes
    void registerRoutes(crow::SimpleApp& app);
};
