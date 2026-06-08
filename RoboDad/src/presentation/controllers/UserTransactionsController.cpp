#include "presentation/controllers/UserTransactionsController.h"
#include "application/users/dtos/TransactionDto.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/queries/GetUserProfileQuery.h"

#include "application/users/commands/CreateTransactionCommand.h"
#include "application/users/commands/DeleteTransactionCommand.h"
#include "application/users/commands/ImportPlaidTransactionsCommand.h"
#include "application/users/commands/UpdateTransactionCommand.h"
#include "application/users/queries/GetTransactionQuery.h"
#include "application/users/queries/ListTransactionsQuery.h"
#include "application/users/queries/ListTransactionsByCategoryQuery.h"

#include <chrono>
#include <sstream>

UserTransactionsController::UserTransactionsController(const CreateTransaction& createTransactions, const DeleteTransaction& deleteTransactions, const GetTransaction& getTransactions, const GetUserProfile& getUserProfile, const ListTransactions& listTransactions, const ListTransactionsByCategory& listTransactionsByCategory, const UpdateTransaction& updateTransactions, const ImportPlaidTransactions& importPlaidTransactions)
    : createTransactions_(createTransactions), deleteTransactions_(deleteTransactions), getTransactions_(getTransactions), getUserProfile_(getUserProfile), listTransactions_(listTransactions), listTransactionsByCategory_(listTransactionsByCategory), updateTransactions_(updateTransactions), importPlaidTransactions_(importPlaidTransactions) {}

void UserTransactionsController::registerRoutes(RoboDadApp& app) {
    CROW_ROUTE(app, "/user/transactions")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req) {
            return getTransactions(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
    CROW_ROUTE(app, "/user/transactions/<string>")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& transaction_id){
            return getTransactionDetails(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionId(transaction_id));
        });
    CROW_ROUTE(app, "/user/transactions/category/<string>")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& category_id) {
            return getTransactionsByCategory(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionCategoryId(category_id));
        });

    CROW_ROUTE(app, "/user/transactions/category/<string>/new")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& category_id) {
            return getNewTransactionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionCategoryId(category_id));
        });
    CROW_ROUTE(app, "/user/transactions/category/<string>/new")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& category_id) {
            return postNewTransaction(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionCategoryId(category_id));
        });

    CROW_ROUTE(app, "/user/transactions/<string>/edit")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& transaction_id) {
            return getEditTransactionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionId(transaction_id));
        });
    CROW_ROUTE(app, "/user/transactions/<string>/edit")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& transaction_id) {
            return postEditTransaction(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionId(transaction_id));
        });

    CROW_ROUTE(app, "/user/transactions/<string>/delete")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req, const std::string& transaction_id) {
            return getDeleteTransactionPage(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionId(transaction_id));
        });
    CROW_ROUTE(app, "/user/transactions/<string>/delete")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& transaction_id) {
            return postDeleteTransaction(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionId(transaction_id));
        });

    CROW_ROUTE(app, "/user/transactions/import/plaid")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req) {
            return getPlaidImportPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
    CROW_ROUTE(app, "/user/transactions/import/plaid")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req) {
            return postPlaidImport(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
}

crow::response UserTransactionsController::getTransactions(const crow::request& req, UserId user_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::vector<TransactionDto> transactions = listTransactions_.execute(ListTransactionsQuery(user_id));
    crow::json::wvalue::list transactionList;
    for (const TransactionDto& t : transactions) {
        transactionList.push_back(static_cast<crow::json::wvalue>(t));
    }
    crow::mustache::context ctx;
    ctx["transactions"] = std::move(transactionList);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("transactions.html").render(ctx));
}

crow::response UserTransactionsController::getTransactionDetails(const crow::request& req, UserId user_id, TransactionId transaction_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<TransactionDto> transactionOpt = getTransactions_.execute(GetTransactionQuery(transaction_id));
    if (!transactionOpt || transactionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Transaction not found");
    }
    crow::mustache::context ctx;
    ctx["transaction"] = static_cast<crow::json::wvalue>(*transactionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("transaction_details.html").render(ctx));
}

crow::response UserTransactionsController::getTransactionsByCategory(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::vector<TransactionDto> transactions = listTransactionsByCategory_.execute(ListTransactionsByCategoryQuery(user_id, category_id));
    crow::json::wvalue::list transactionList;
    for (const TransactionDto& t : transactions) {
        transactionList.push_back(static_cast<crow::json::wvalue>(t));
    }
    crow::mustache::context ctx;
    ctx["transactions"] = std::move(transactionList);
    ctx["category_id"] = category_id.getId();
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    std::string tmpl = "transactions_by_category.html";
    if (category_id.getId() == "income")   tmpl = "earnings.html";
    if (category_id.getId() == "expenses") tmpl = "expenses.html";
    return crow::response(crow::mustache::load(tmpl).render(ctx));
}

crow::response UserTransactionsController::getNewTransactionPage(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    crow::mustache::context ctx;
    ctx["category_id"] = category_id.getId();
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    std::string tmpl = "new_transaction.html";
    if (category_id.getId() == "income")   tmpl = "new_earning.html";
    if (category_id.getId() == "expenses") tmpl = "new_expense.html";
    return crow::response(crow::mustache::load(tmpl).render(ctx));
}

crow::response UserTransactionsController::getEditTransactionPage(const crow::request& req, UserId user_id, TransactionId transaction_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<TransactionDto> transactionOpt = getTransactions_.execute(GetTransactionQuery(transaction_id));
    if (!transactionOpt || transactionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Transaction not found");
    }
    crow::mustache::context ctx;
    ctx["transaction"] = static_cast<crow::json::wvalue>(*transactionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    const std::string& catId = transactionOpt->getCategoryId();
    std::string tmpl = "edit_transaction.html";
    if (catId == "income")   tmpl = "edit_earning.html";
    if (catId == "expenses") tmpl = "edit_expense.html";
    return crow::response(crow::mustache::load(tmpl).render(ctx));
}

crow::response UserTransactionsController::getDeleteTransactionPage(const crow::request& req, UserId user_id, TransactionId transaction_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    std::optional<TransactionDto> transactionOpt = getTransactions_.execute(GetTransactionQuery(transaction_id));
    if (!transactionOpt || transactionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Transaction not found");
    }
    crow::mustache::context ctx;
    ctx["transaction"] = static_cast<crow::json::wvalue>(*transactionOpt);
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    const std::string& catId = transactionOpt->getCategoryId();
    std::string tmpl = "delete_transaction.html";
    if (catId == "income")   tmpl = "delete_earning.html";
    if (catId == "expenses") tmpl = "delete_expense.html";
    return crow::response(crow::mustache::load(tmpl).render(ctx));
}

static std::chrono::year_month_day parseDate(const std::string& s) {
    std::istringstream ss(s);
    std::chrono::year_month_day ymd;
    ss >> std::chrono::parse("%Y-%m-%d", ymd);
    return ymd;
}

crow::response UserTransactionsController::postNewTransaction(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    crow::query_string params("?" + req.body);
    std::string amountStr   = params.get("amount")      ? params.get("amount")      : "";
    std::string currencyId  = params.get("currency_id") ? params.get("currency_id") : "";
    std::string description = params.get("description") ? params.get("description") : "";
    std::string date        = params.get("date")        ? params.get("date")        : "";

    if (amountStr.empty() || description.empty() || date.empty()) {
        return crow::response(400, "Missing required fields");
    }

    CreateTransactionCommand createRequest(
        user_id,
        category_id,
        std::stod(amountStr),
        currencyId.empty() ? std::nullopt : std::optional<CurrencyId>{CurrencyId(currencyId)},
        description,
        parseDate(date)
    );
    createTransactions_.execute(createRequest);
    crow::response res(302);
    res.add_header("Location", "/user/transactions/category/" + category_id.getId());
    return res;
}

crow::response UserTransactionsController::postEditTransaction(const crow::request& req, UserId user_id, TransactionId transaction_id) {
    crow::query_string params("?" + req.body);
    std::string categoryId  = params.get("category_id") ? params.get("category_id") : "";
    std::string amountStr   = params.get("amount")      ? params.get("amount")      : "";
    std::string currencyId  = params.get("currency_id") ? params.get("currency_id") : "";
    std::string description = params.get("description") ? params.get("description") : "";
    std::string date        = params.get("date")        ? params.get("date")        : "";

    if (categoryId.empty() || amountStr.empty() || description.empty() || date.empty()) {
        return crow::response(400, "Missing required fields");
    }

    UpdateTransactionCommand updateRequest(
        transaction_id,
        TransactionCategoryId(categoryId),
        std::stod(amountStr),
        currencyId.empty() ? std::nullopt : std::optional<CurrencyId>{CurrencyId(currencyId)},
        description,
        parseDate(date)
    );
    updateTransactions_.execute(updateRequest);
    crow::response res(302);
    res.add_header("Location", "/user/transactions/" + transaction_id.getId());
    return res;
}

crow::response UserTransactionsController::postDeleteTransaction(const crow::request& req, UserId user_id, TransactionId transaction_id) {
    deleteTransactions_.execute(DeleteTransactionCommand(transaction_id));
    crow::response res(302);
    res.add_header("Location", "/user/transactions");
    return res;
}

crow::response UserTransactionsController::getPlaidImportPage(const crow::request& req, UserId user_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    crow::mustache::context ctx;
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    return crow::response(crow::mustache::load("import_plaid.html").render(ctx));
}

crow::response UserTransactionsController::postPlaidImport(const crow::request& req, UserId user_id) {
    crow::query_string params("?" + req.body);
    std::string accessToken = params.get("access_token") ? params.get("access_token") : "";
    std::string startDate   = params.get("start_date")   ? params.get("start_date")   : "";
    std::string endDate     = params.get("end_date")     ? params.get("end_date")     : "";

    if (accessToken.empty() || startDate.empty() || endDate.empty()) {
        return crow::response(400, "Missing required fields");
    }

    ImportPlaidTransactionsCommand importCommand(user_id, accessToken, startDate, endDate);
    importPlaidTransactions_.execute(importCommand);
    return crow::response(200, "Transactions imported successfully");
}
