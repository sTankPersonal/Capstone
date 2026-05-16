#pragma once
#include <crow.h>
#include <cstdint>
#include <optional>

// Forward declarations — avoids pulling every use-case header into every TU
// that includes HttpServer.h.
class LoginUser;
class RegisterUser;
class AddExpense;
class ListExpenses;
class SetBudgetGoal;
class GetBudgetStatus;
class GetBudgetFeedback;
class JwtService;

// Owns the Crow application and all use-case references.
// Responsible for registering every route and starting the server.
class HttpServer {
    crow::SimpleApp app_;

    LoginUser&        loginUser_;
    RegisterUser&     registerUser_;
    AddExpense&       addExpense_;
    ListExpenses&     listExpenses_;
    SetBudgetGoal&    setBudgetGoal_;
    GetBudgetStatus&  getBudgetStatus_;
    GetBudgetFeedback& getBudgetFeedback_;
    JwtService&       jwtService_;

public:
    HttpServer(LoginUser&, RegisterUser&,
               AddExpense&, ListExpenses&,
               SetBudgetGoal&, GetBudgetStatus&,
               GetBudgetFeedback&, JwtService&);

    // Registers all route groups and begins Crow's event loop on the given port.
    // Blocks until the server shuts down.
    void start(uint16_t port);

private:
    // Registers POST /auth/register and POST /auth/login.
    void registerAuthRoutes();

    // Registers POST /expenses and GET /expenses/<userId>.
    void registerExpenseRoutes();

    // Registers POST /budgets and GET /budgets/<userId>.
    void registerBudgetRoutes();

    // Registers POST /ai/feedback for LLM-generated budget advice.
    void registerAiRoutes();

    // Validates "Authorization: Bearer <token>" using JwtService.
    // Returns the authenticated userId on success, or nullopt if the token
    // is absent, malformed, or fails verification.
    std::optional<uint32_t> authenticate(const crow::request& req) const;
};
