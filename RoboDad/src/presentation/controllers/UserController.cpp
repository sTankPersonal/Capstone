#include "presentation/controllers/UserController.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/queries/GetUserProfileQuery.h"
#include "application/users/commands/UpdateUserPasswordCommand.h"
#include "application/users/commands/UpdateUserProfileCommand.h"
#include "domain/valueObjects/UserInformation.h"

UserController::UserController(const GetUserProfile& getUserProfile, const UpdateUserProfile& updateUserProfile, const UpdateUserPassword& updateUserPassword)
    : getUserProfile_(getUserProfile), updateUserProfile_(updateUserProfile), updateUserPassword_(updateUserPassword) {}

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
            return postDeleteUserSettings(req, UserId(app.get_context<AuthMiddleware>(req).userId));
        });
}

crow::response UserController::getUserDashboardPage(const crow::request& req, UserId user_id){
    std::optional<UserProfileDto> userOpt = getUserProfile_.execute(GetUserProfileQuery(user_id));
    if (!userOpt) {
        std::cerr << "User with ID " << user_id.getId() << " not found." << std::endl;
        return crow::response(404, "User not found");
    }

    crow::mustache::context ctx;
    ctx["user"] = static_cast<crow::json::wvalue>(*userOpt);

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
    std::string newEmail    = params.get("email")    ? params.get("email")    : "";
    std::string newPassword = params.get("password") ? params.get("password") : "";

    if (newEmail.empty() || newPassword.empty()) {
        return crow::response(400, "Missing required fields");
    }

    try {
        updateUserPassword_.execute(UpdateUserPasswordCommand(user_id, "", newPassword));
        return crow::response(200, "User login information updated successfully");
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
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

    try {
        updateUserProfile_.execute(UpdateUserProfileCommand(
            user_id,
            UserInformation(
                newFirstName.empty()        ? std::nullopt : std::optional<std::string>{newFirstName},
                newLastName.empty()         ? std::nullopt : std::optional<std::string>{newLastName},
                std::nullopt,
                newCountry.empty()          ? std::nullopt : std::optional<CountryId>{CountryId(newCountry)},
                newCurrency.empty()         ? std::nullopt : std::optional<CurrencyId>{CurrencyId(newCurrency)},
                newLanguage.empty()         ? std::nullopt : std::optional<LanguageId>{LanguageId(newLanguage)},
                newEmploymentStatus.empty() ? std::nullopt : std::optional<EmploymentStatusId>{EmploymentStatusId(newEmploymentStatus)}
            )
        ));
        return crow::response(200, "User information updated successfully");
    } catch (const std::exception& e) {
        return crow::response(500, e.what());
    }
}

crow::response UserController::postDeleteUserSettings(const crow::request& req, UserId user_id){
    return crow::response(200, "User deletion functionality not implemented yet");
}
