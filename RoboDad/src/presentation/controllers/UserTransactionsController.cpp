#include "presentation/controllers/UserTransactionsController.h"
#include "application/users/dtos/TransactionDto.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/queries/GetUserProfileQuery.h"

#include "application/users/commands/CreateTransactionCommand.h"
#include "application/users/commands/DeleteTransactionCommand.h"
#include "application/users/commands/ImportPlaidTransactionsCommand.h"
#include "application/users/commands/ImportCsvTransactionsCommand.h"
#include "application/users/commands/UpdateTransactionCommand.h"
#include "application/users/queries/GetTransactionQuery.h"
#include "application/users/queries/ListTransactionsQuery.h"
#include "application/users/queries/ListTransactionsByCategoryQuery.h"

#include <chrono>
#include <sstream>

UserTransactionsController::UserTransactionsController(const CreateTransaction& createTransactions, const DeleteTransaction& deleteTransactions, const GetTransaction& getTransactions, const GetUserProfile& getUserProfile, const ListTransactions& listTransactions, const ListTransactionsByCategory& listTransactionsByCategory, const UpdateTransaction& updateTransactions, const ImportPlaidTransactions& importPlaidTransactions, const ImportCsvTransactions& importCsvTransactions)
    : createTransactions_(createTransactions), deleteTransactions_(deleteTransactions), getTransactions_(getTransactions), getUserProfile_(getUserProfile), listTransactions_(listTransactions), listTransactionsByCategory_(listTransactionsByCategory), updateTransactions_(updateTransactions), importPlaidTransactions_(importPlaidTransactions), importCsvTransactions_(importCsvTransactions) {}

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

    CROW_ROUTE(app, "/user/transactions/category/<string>/import/csv")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& category_id) {
            return postCsvImport(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionCategoryId(category_id));
        });

    CROW_ROUTE(app, "/user/transactions/category/<string>/delete")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req, const std::string& category_id) {
            return postBulkDelete(req, UserId(app.get_context<AuthMiddleware>(req).userId), TransactionCategoryId(category_id));
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

    const std::string& catId = category_id.getId();
    if (catId != "earnings" && catId != "expenses") return crow::response(404, "Unknown category");
    std::string tmpl = (catId == "earnings") ? "earnings.html" : "expenses.html";
    return crow::response(crow::mustache::load(tmpl).render(ctx));
}

crow::response UserTransactionsController::getNewTransactionPage(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    const std::string& catId = category_id.getId();
    if (catId != "earnings" && catId != "expenses") return crow::response(404, "Unknown category");
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    crow::mustache::context ctx;
    ctx["category_id"] = catId;
    if (userOpt) ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);
    std::string tmpl = (catId == "earnings") ? "new_earning.html" : "new_expense.html";
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
    std::string tmpl = "edit_expense.html";
    if (catId == "earnings") tmpl = "edit_earning.html";
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
    std::string tmpl = "delete_expense.html";
    if (catId == "earnings") tmpl = "delete_earning.html";
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
    std::optional<TransactionDto> transactionOpt = getTransactions_.execute(GetTransactionQuery(transaction_id));
    if (!transactionOpt || transactionOpt->getUserId() != user_id.getId()) {
        return crow::response(404, "Transaction not found");
    }
    std::string categoryId = transactionOpt->getCategoryId();
    deleteTransactions_.execute(DeleteTransactionCommand(transaction_id));
    crow::response res(302);
    res.add_header("Location", "/user/transactions/category/" + categoryId);
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

crow::response UserTransactionsController::postCsvImport(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    if (req.body.empty()) {
        return crow::response(400, "Empty file");
    }

    CsvImportFlow flow = (category_id.getId() == "earnings") ? CsvImportFlow::MoneyIn : CsvImportFlow::MoneyOut;
    ImportCsvTransactionsCommand importCommand(user_id, category_id, req.body, flow);
    std::vector<Transaction> imported = importCsvTransactions_.execute(importCommand);

    crow::json::wvalue result;
    result["imported"] = static_cast<int>(imported.size());
    return crow::response(200, std::move(result));
}

crow::response UserTransactionsController::postBulkDelete(const crow::request& req, UserId user_id, TransactionCategoryId category_id) {
    crow::query_string params("?" + req.body);
    std::string idsStr = params.get("ids") ? params.get("ids") : "";
    if (idsStr.empty()) {
        return crow::response(400, "No transactions selected");
    }

    int deleted = 0;
    std::stringstream ss(idsStr);
    std::string id;
    while (std::getline(ss, id, ',')) {
        if (id.empty()) continue;
        TransactionId transactionId(id);
        std::optional<TransactionDto> transactionOpt = getTransactions_.execute(GetTransactionQuery(transactionId));
        if (transactionOpt && transactionOpt->getUserId() == user_id.getId()) {
            if (deleteTransactions_.execute(DeleteTransactionCommand(transactionId))) {
                ++deleted;
            }
        }
    }

    crow::json::wvalue result;
    result["deleted"] = deleted;
    return crow::response(200, std::move(result));
}
