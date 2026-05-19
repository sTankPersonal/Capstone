#include "infrastructure/http/HttpServer.h"
#include "application/auth/LoginUser.h"
#include "application/auth/RegisterUser.h"
#include "application/expenses/AddExpense.h"
#include "application/expenses/ListExpenses.h"
#include "application/budgets/SetBudgetGoal.h"
#include "application/budgets/GetBudgetStatus.h"
#include "application/ai/GetBudgetFeedback.h"
#include "infrastructure/security/JwtService.h"
#include "infrastructure/http/HttpRequest.h"
#include "infrastructure/http/HttpResponse.h"

HttpServer::HttpServer(LoginUser& lu, RegisterUser& ru,
                       AddExpense& ae, ListExpenses& le,
                       SetBudgetGoal& sbg, GetBudgetStatus& gbs,
                       GetBudgetFeedback& gbf, JwtService& jwt)
    : loginUser_(lu), registerUser_(ru),
      addExpense_(ae), listExpenses_(le),
      setBudgetGoal_(sbg), getBudgetStatus_(gbs),
      getBudgetFeedback_(gbf), jwtService_(jwt) {}

void HttpServer::start(uint16_t port) {
    // TODO: call registerAuthRoutes(), registerExpenseRoutes(),
    //       registerBudgetRoutes(), registerAiRoutes();
    //       then call app_.port(port).multithreaded().run()
}

void HttpServer::registerAuthRoutes() {
    // POST /auth/register
    // TODO: define CROW_ROUTE(app_, "/auth/register").methods(crow::HTTPMethod::POST);
    //       wrap req in HttpRequest, extract PersonalInfo fields from JSON body,
    //       call registerUser_.execute(info), serialize the returned User to JSON,
    //       return HttpResponse::created(...).toCrowResponse()

    // POST /auth/login
    // TODO: define CROW_ROUTE(app_, "/auth/login").methods(crow::HTTPMethod::POST);
    //       extract userId from JSON body, call loginUser_.execute(userId),
    //       if nullopt return HttpResponse::notFound("user not found"),
    //       otherwise generate a JWT via jwtService_.generate(user.getId())
    //       and return HttpResponse::ok({"token": jwt}).toCrowResponse()
}

void HttpServer::registerExpenseRoutes() {
    // POST /expenses
    // TODO: define route, call authenticate(req); return 401 if nullopt;
    //       extract amount, description, type from JSON body,
    //       call addExpense_.execute(userId, amount, desc, type),
    //       return HttpResponse::created(serialized Finance).toCrowResponse()

    // GET /expenses/<userId>
    // TODO: define route with <uint32_t> path param;
    //       call authenticate(req); return 401 if nullopt;
    //       call listExpenses_.execute(userId),
    //       serialize the vector<Finance> to a JSON array,
    //       return HttpResponse::ok(array).toCrowResponse()
}

void HttpServer::registerBudgetRoutes() {
    // POST /budgets
    // TODO: define route, call authenticate(req); return 401 if nullopt;
    //       extract endGoal, amount, description from JSON body,
    //       call setBudgetGoal_.execute(userId, endGoal, amount, desc),
    //       return HttpResponse::created(serialized BudgetGoal).toCrowResponse()

    // GET /budgets/<userId>
    // TODO: define route with <uint32_t> path param;
    //       call authenticate(req); return 401 if nullopt;
    //       call getBudgetStatus_.execute(userId),
    //       serialize the vector<BudgetGoal> to a JSON array,
    //       return HttpResponse::ok(array).toCrowResponse()
}

void HttpServer::registerAiRoutes() {
    // POST /ai/feedback
    // TODO: define route, call authenticate(req); return 401 if nullopt;
    //       extract "message" string from JSON body,
    //       call getBudgetFeedback_.execute(userId, message),
    //       return HttpResponse::ok({"response": llmText}).toCrowResponse();
    //       catch exceptions and return HttpResponse::internalError(e.what())
}

std::optional<uint32_t> HttpServer::authenticate(const crow::request& req) const {
    // TODO: wrap req in HttpRequest, call getBearerToken();
    //       if nullopt return nullopt;
    //       call jwtService_.verify(token) and return its result directly
    //       (verify already returns optional<uint32_t>)
    return std::nullopt;
}
