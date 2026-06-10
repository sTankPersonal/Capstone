#include "presentation/controllers/UserController.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/queries/GetUserProfileQuery.h"
#include "application/users/commands/UpdateUserPasswordCommand.h"
#include "application/users/commands/UpdateUserProfileCommand.h"
#include "application/users/commands/DeleteUserCommand.h"
#include "application/users/queries/GetFinancialInsightsQuery.h"
#include "application/users/dtos/FinancialInsightsDto.h"
#include "domain/valueObjects/UserInformation.h"

UserController::UserController(const GetUserProfile& getUserProfile, const UpdateUserProfile& updateUserProfile, const UpdateUserPassword& updateUserPassword, const DeleteUser& deleteUser, const GetFinancialInsights& getFinancialInsights)
    : getUserProfile_(getUserProfile), updateUserProfile_(updateUserProfile), updateUserPassword_(updateUserPassword), deleteUser_(deleteUser), getFinancialInsights_(getFinancialInsights) {}

void UserController::registerRoutes(RoboDadApp &app){
    CROW_ROUTE(app, "/user/dashboard")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getUserDashboardPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getUserSettingsPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings/userLogin")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getUserSettingsLoginPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings/userInformation")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getUserSettingsInformationPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings/userLogin/edit")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getEditUserSettingsLoginPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
    CROW_ROUTE(app, "/user/settings/userLogin/edit")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postEditUserSettingsLogin(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings/userInformation/edit")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getEditUserSettingsInformationPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
    CROW_ROUTE(app, "/user/settings/userInformation/edit")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postEditUserSettingsInformation(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });

    CROW_ROUTE(app, "/user/settings/delete")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getDeleteUserSettingsPage(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
    CROW_ROUTE(app, "/user/settings/delete")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postDeleteUserSettings(req, UserId(app.get_context<AuthMiddleware>(req).userId), app);
        });
}
crow::response UserController::getUserDashboardPage(const crow::request& req, UserId user_id) {
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        std::cerr << "User with ID " << user_id.getId() << " not found." << std::endl;
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    auto insightsOpt = getFinancialInsights_.execute(GetFinancialInsightsQuery(user_id));
    if (insightsOpt) {
        crow::json::wvalue insightsJson;

        // Income totals
        insightsJson["totalIncome"] = insightsOpt->totalIncome;

        // Income by category → list of {category, amount, count}
        {
            crow::json::wvalue::list incomeList;
            for (const auto& item : insightsOpt->incomeByCategoryList) {
                crow::json::wvalue entry;
                entry["category"] = item.category;
                entry["amount"] = item.amount;
                entry["count"] = item.count;
                incomeList.push_back(std::move(entry));
            }
            insightsJson["incomeByCategoryList"] = std::move(incomeList);
        }

        // Build income arrays for Chart.js
        {
            crow::json::wvalue::list labels;
            crow::json::wvalue::list values;

            for (const auto& item : insightsOpt->incomeByCategoryList) {
                labels.push_back(item.category);
                values.push_back(item.amount);
            }

            insightsJson["incomeLabels"] = std::move(labels);
            insightsJson["incomeValues"] = std::move(values);
        }

        // Unusual income → list of strings
        {
            crow::json::wvalue::list unusualIncomeList;
            for (const auto& desc : insightsOpt->unusualIncome) {
                unusualIncomeList.push_back(desc);
            }
            insightsJson["unusualIncome"] = std::move(unusualIncomeList);
        }

        // Expense totals
        insightsJson["totalExpenses"] = insightsOpt->totalExpenses;

        // Expenses by category → list of {category, amount, count}
        {
            crow::json::wvalue::list expenseList;
            for (const auto& item : insightsOpt->expenseByCategoryList) {
                crow::json::wvalue entry;
                entry["category"] = item.category;
                entry["amount"] = item.amount;
                entry["count"] = item.count;
                expenseList.push_back(std::move(entry));
            }
            insightsJson["expenseByCategoryList"] = std::move(expenseList);
        }

        // Build expense arrays for Chart.js
        {
            crow::json::wvalue::list labels;
            crow::json::wvalue::list values;

            for (const auto& item : insightsOpt->expenseByCategoryList) {
                labels.push_back(item.category);
                values.push_back(item.amount);
            }

            insightsJson["expenseLabels"] = std::move(labels);
            insightsJson["expenseValues"] = std::move(values);
        }

        // Unusual expenses → list of strings
        {
            crow::json::wvalue::list unusualExpenseList;
            for (const auto& desc : insightsOpt->unusualExpenses) {
                unusualExpenseList.push_back(desc);
            }
            insightsJson["unusualExpenses"] = std::move(unusualExpenseList);
        }

        ctx["insights"] = std::move(insightsJson);
    }

    return crow::response(crow::mustache::load("user_dashboard.html").render(ctx));
}




crow::response UserController::getUserSettingsPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("user_settings.html").render(ctx));
}

crow::response UserController::getUserSettingsLoginPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("user_settings_login.html").render(ctx));
}

crow::response UserController::getUserSettingsInformationPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("user_settings_information.html").render(ctx));
}

crow::response UserController::getEditUserSettingsLoginPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("edit_user_settings_login.html").render(ctx));
}

crow::response UserController::getEditUserSettingsInformationPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("edit_user_settings_information.html").render(ctx));
}

crow::response UserController::getDeleteUserSettingsPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

    return crow::response(crow::mustache::load("delete_user_settings.html").render(ctx));
}

crow::response UserController::postEditUserSettingsLogin(const crow::request& req, UserId user_id){
    crow::query_string params("?" + req.body);
    std::string currentPassword = params.get("current_password") ? params.get("current_password") : "";
    std::string newPassword     = params.get("new_password")     ? params.get("new_password")     : "";
    std::string confirmPassword = params.get("confirm_password") ? params.get("confirm_password") : "";

    if (currentPassword.empty() || newPassword.empty()) {
        return crow::response(400, "Missing required fields");
    }
    if (newPassword != confirmPassword) {
        return crow::response(400, "Passwords do not match");
    }

    bool success = updateUserPassword_.execute(UpdateUserPasswordCommand(user_id, currentPassword, newPassword));
    if (!success) {
        return crow::response(401, "Current password is incorrect");
    }

    crow::response res(302);
    res.add_header("Location", "/user/settings/userLogin");
    return res;
}

crow::response UserController::postEditUserSettingsInformation(const crow::request& req, UserId user_id){
    crow::query_string params("?" + req.body);
    std::string newFirstName        = params.get("first_name")         ? params.get("first_name")         : "";
    std::string newLastName         = params.get("last_name")          ? params.get("last_name")          : "";
    std::string newDateOfBirth      = params.get("date_of_birth")      ? params.get("date_of_birth")      : "";
    std::string newCountry          = params.get("country")            ? params.get("country")            : "";
    std::string newCurrency         = params.get("currency")           ? params.get("currency")           : "";
    std::string newLanguage         = params.get("language")           ? params.get("language")           : "";
    std::string newEmploymentStatus = params.get("employment_status")  ? params.get("employment_status")  : "";

    std::optional<std::chrono::year_month_day> dob = std::nullopt;
    if (!newDateOfBirth.empty()) {
        int y = 0, m = 0, d = 0;
        if (std::sscanf(newDateOfBirth.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
            dob = std::chrono::year_month_day{
                std::chrono::year{y},
                std::chrono::month{static_cast<unsigned>(m)},
                std::chrono::day{static_cast<unsigned>(d)}
            };
        }
    }

    try {
        updateUserProfile_.execute(UpdateUserProfileCommand(
            user_id,
            UserInformation(
                newFirstName.empty()        ? std::nullopt : std::optional<std::string>{newFirstName},
                newLastName.empty()         ? std::nullopt : std::optional<std::string>{newLastName},
                dob,
                newCountry.empty()          ? std::nullopt : std::optional<CountryId>{CountryId(newCountry)},
                newCurrency.empty()         ? std::nullopt : std::optional<CurrencyId>{CurrencyId(newCurrency)},
                newLanguage.empty()         ? std::nullopt : std::optional<LanguageId>{LanguageId(newLanguage)},
                newEmploymentStatus.empty() ? std::nullopt : std::optional<EmploymentStatusId>{EmploymentStatusId(newEmploymentStatus)}
            )
        ));
        crow::response res(302);
        res.add_header("Location", "/user/settings/userInformation");
        return res;
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
}

crow::response UserController::postDeleteUserSettings(const crow::request& req, UserId user_id, RoboDadApp& app){
    bool success = deleteUser_.execute(DeleteUserCommand(user_id));
    if (!success) {
        return crow::response(404, "User not found");
    }

    auto& ctx = app.get_context<crow::CookieParser>(req);
    ctx.set_cookie("userId", "").path("/").max_age(0);

    crow::response res(302);
    res.add_header("Location", "/");
    return res;
}
