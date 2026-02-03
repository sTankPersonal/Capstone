#include "presentation/controllers/ActivityController.h"
#include "domain/valueObjects/ids/UserId.h"
#include "domain/valueObjects/ids/TransactionId.h"
#include "domain/valueObjects/ids/TransactionCategoryId.h"
#include "domain/valueObjects/ids/CurrencyId.h"
#include <crow.h>
#include <chrono>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

ActivityController::ActivityController(ITransactionRepository&         transactionRepo,
                                       ITransactionCategoryRepository& categoryRepo,
                                       ICurrencyRepository&            currencyRepo,
                                       IPlaidClient&                   plaidClient,
                                       IJwtService&                    jwt)
    : createTransaction_(transactionRepo),
      deleteTransaction_(transactionRepo),
      getTransaction_(transactionRepo),
      importTransactions_(transactionRepo, categoryRepo, currencyRepo, plaidClient),
      listTransactions_(transactionRepo),
      updateTransaction_(transactionRepo),
      jwt_(jwt) {}

std::optional<std::string> ActivityController::extractUserId(const crow::request& req) const {
    std::string cookie = req.get_header_value("Cookie");
    const std::string prefix = "token=";
    auto pos = cookie.find(prefix);
    if (pos == std::string::npos) return std::nullopt;
    pos += prefix.size();
    auto end = cookie.find(';', pos);
    std::string token = (end == std::string::npos)
        ? cookie.substr(pos)
        : cookie.substr(pos, end - pos);
    return jwt_.verify(token);
}

static std::string formatDate(const std::chrono::year_month_day& d) {
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%04d-%02u-%02u",
                  static_cast<int>(d.year()),
                  static_cast<unsigned>(d.month()),
                  static_cast<unsigned>(d.day()));
    return buf;
}

static std::optional<std::chrono::year_month_day> parseDate(const std::string& s) {
    if (s.size() != 10 || s[4] != '-' || s[7] != '-') return std::nullopt;
    try {
        int      y = std::stoi(s.substr(0, 4));
        unsigned m = static_cast<unsigned>(std::stoi(s.substr(5, 2)));
        unsigned d = static_cast<unsigned>(std::stoi(s.substr(8, 2)));
        return std::chrono::year_month_day{
            std::chrono::year{y}, std::chrono::month{m}, std::chrono::day{d}
        };
    } catch (...) { return std::nullopt; }
}

static crow::json::wvalue transactionToJson(const Transaction& t) {
    crow::json::wvalue j;
    j["id"]         = t.getId().getId();
    j["userId"]     = t.getUserId().getId();
    j["categoryId"] = t.getCategoryId().getId();
    j["amount"]     = t.getAmount().getAmount().value_or(0.0);
    if (const auto& cid = t.getAmount().getCurrencyId(); cid)
        j["currencyId"] = cid->getId();
    j["description"] = t.getDescription().getDescription().value_or(std::string{});
    j["date"]        = formatDate(t.getTransactionDate());
    j["createdAt"]   = formatDate(t.getCreatedAt());
    return j;
}

void ActivityController::registerRoutes(crow::SimpleApp& app) {
    //list all earnings
    CROW_ROUTE(app, "/earnings")
    ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/earnings.html");
        res.end();
    });
    //list singular earning by id
    CROW_ROUTE(app, "/earning/<string>")
        ([](const crow::request&, crow::response& res, const std::string& id) {
        res.set_static_file_info("public/earning.html");
        res.end();
            });
    //list all expenses
    CROW_ROUTE(app, "/expenses")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/expenses.html");
        res.end();
            });
    //list singular expense by id
    CROW_ROUTE(app, "/expense/<string>")
        ([](const crow::request&, crow::response& res, const std::string& id) {
        res.set_static_file_info("public/expense.html");
        res.end();
            });

    // GET /api/transactions
    CROW_ROUTE(app, "/api/transactions").methods("GET"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        try {
            auto txns = listTransactions_.execute(UserId(*userId));
            std::vector<crow::json::wvalue> items;
            items.reserve(txns.size());
            for (auto& t : txns)
                items.push_back(transactionToJson(t));
            return crow::response(200, crow::json::wvalue(std::move(items)).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // POST /api/transactions
    CROW_ROUTE(app, "/api/transactions").methods("POST"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("categoryId") || !body.has("amount") ||
            !body.has("description") || !body.has("date"))
            return crow::response(400, "Missing required fields");

        auto date = parseDate(body["date"].s());
        if (!date) return crow::response(400, "Invalid date format, expected YYYY-MM-DD");

        std::optional<CurrencyId> currencyId;
        if (body.has("currencyId"))
            currencyId = CurrencyId(body["currencyId"].s());

        try {
            auto tx = createTransaction_.execute(
                UserId(*userId),
                TransactionCategoryId(body["categoryId"].s()),
                body["amount"].d(),
                currencyId,
                body["description"].s(),
                *date
            );
            return crow::response(201, transactionToJson(tx).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // GET /api/transactions/:id
    CROW_ROUTE(app, "/api/transactions/<string>").methods("GET"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            auto tx = getTransaction_.execute(TransactionId(id));
            if (!tx) return crow::response(404, "Transaction not found");
            return crow::response(200, transactionToJson(*tx).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // PUT /api/transactions/:id
    CROW_ROUTE(app, "/api/transactions/<string>").methods("PUT"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("categoryId") || !body.has("amount") ||
            !body.has("description") || !body.has("date"))
            return crow::response(400, "Missing required fields");

        auto date = parseDate(body["date"].s());
        if (!date) return crow::response(400, "Invalid date format, expected YYYY-MM-DD");

        std::optional<CurrencyId> currencyId;
        if (body.has("currencyId"))
            currencyId = CurrencyId(body["currencyId"].s());

        try {
            bool ok = updateTransaction_.execute(
                TransactionId(id),
                TransactionCategoryId(body["categoryId"].s()),
                body["amount"].d(),
                currencyId,
                body["description"].s(),
                *date
            );
            return ok ? crow::response(204) : crow::response(404, "Transaction not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // DELETE /api/transactions/:id
    CROW_ROUTE(app, "/api/transactions/<string>").methods("DELETE"_method)
    ([this](const crow::request& req, const std::string& id) {
        if (!extractUserId(req)) return crow::response(401, "Unauthorized");

        try {
            bool ok = deleteTransaction_.execute(TransactionId(id));
            return ok ? crow::response(204) : crow::response(404, "Transaction not found");
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });

    // POST /api/transactions/import — import from Plaid sandbox
    CROW_ROUTE(app, "/api/transactions/import").methods("POST"_method)
    ([this](const crow::request& req) {
        auto userId = extractUserId(req);
        if (!userId) return crow::response(401, "Unauthorized");

        auto body = crow::json::load(req.body);
        if (!body || !body.has("startDate") || !body.has("endDate"))
            return crow::response(400, "Missing 'startDate' or 'endDate'");

        int count = body.has("count") ? static_cast<int>(body["count"].i()) : 100;

        try {
            auto txns = importTransactions_.execute(
                UserId(*userId),
                body["startDate"].s(),
                body["endDate"].s(),
                count
            );
            std::vector<crow::json::wvalue> items;
            items.reserve(txns.size());
            for (auto& t : txns)
                items.push_back(transactionToJson(t));
            return crow::response(201, crow::json::wvalue(std::move(items)).dump());
        } catch (const std::exception& ex) {
            return crow::response(500, ex.what());
        }
    });
}
