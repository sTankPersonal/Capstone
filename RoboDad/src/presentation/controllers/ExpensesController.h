#pragma once

#include <crow.h>
#include "infrastructureServices/security/IJwtService.h"

class ExpensesController {
public:
    explicit ExpensesController(IJwtService& jwt) : jwt_(jwt) {}
    void registerRoutes(crow::SimpleApp& app);
private:
    IJwtService& jwt_;
};