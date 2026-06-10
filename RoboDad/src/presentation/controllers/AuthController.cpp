#include "presentation/controllers/AuthController.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/commands/LoginUserCommand.h"
#include "application/users/commands/RegisterUserCommand.h"
#include "application/users/commands/OAuthLoginCommand.h"
#include "domain/valueObjects/UserInformation.h"

#include <crow/mustache.h>


AuthController::AuthController(const LoginUser& loginUser,
                               const RegisterUser& registerUser,
                               const LoginOrRegisterOAuthUser& loginOrRegisterOAuthUser,
                               IGoogleOAuthClient& googleOAuth,
                               IJwtService& jwt)
    : loginUser_(loginUser)
    , registerUser_(registerUser)
    , loginOrRegisterOAuthUser_(loginOrRegisterOAuthUser)
    , googleOAuth_(googleOAuth)
    , jwt_(jwt) {}

void AuthController::registerRoutes(RoboDadApp& app) {
    CROW_ROUTE(app, "/auth/login")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthLoginPage(req);
        });
    CROW_ROUTE(app, "/auth/login")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postAuthLogin(req, app);
        });

    CROW_ROUTE(app, "/auth/register")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthRegisterPage(req);
        });
    CROW_ROUTE(app, "/auth/register")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postAuthRegister(req, app);
        });

    CROW_ROUTE(app, "/auth/logout")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthLogout(req);
        });
    CROW_ROUTE(app, "/auth/logout")
        .methods(crow::HTTPMethod::POST)([this, &app](const crow::request& req){
            return postAuthLogout(req, app);
        });

    CROW_ROUTE(app, "/auth/google")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthGoogle(req);
        });

    CROW_ROUTE(app, "/auth/google/callback")
        .methods(crow::HTTPMethod::GET)([this, &app](const crow::request& req){
            return getAuthGoogleCallback(req, app);
        });
}

crow::response AuthController::getAuthLoginPage(const crow::request& req) {
    return crow::response(crow::mustache::load("login.html").render());
}

crow::response AuthController::getAuthRegisterPage(const crow::request& req) {
    return crow::response(crow::mustache::load("register.html").render());
}

crow::response AuthController::getAuthLogout(const crow::request& req) {
    return crow::response(crow::mustache::load("logout.html").render());
}

crow::response AuthController::postAuthLogin(const crow::request& req, RoboDadApp& app) {
    crow::query_string params("?" + req.body);
    std::string email    = params.get("email")    ? params.get("email")    : "";
    std::string password = params.get("password") ? params.get("password") : "";

    if (email.empty() || password.empty()) {
        return crow::response(400, "Missing required fields");
    }

    LoginUserCommand loginCommand{ email, password };
    std::optional<UserProfileDto> user = loginUser_.execute(loginCommand);

    if (!user) {
        return crow::response(401, "Invalid login credentials");
    }
    auto& ctx = app.get_context<crow::CookieParser>(req);
    ctx.set_cookie("userId", jwt_.generate(user->getId()))
        .path("/")
        .max_age(60 * 60 * 24 * 7);
    crow::response res(302);
    res.add_header("Location", "/user/dashboard");
    return res;
}

crow::response AuthController::postAuthRegister(const crow::request& req, RoboDadApp& app) {
    crow::query_string params("?" + req.body);
    std::string firstName = params.get("first_name") ? params.get("first_name") : "";
    std::string lastName  = params.get("last_name")  ? params.get("last_name")  : "";
    std::string email     = params.get("email")      ? params.get("email")      : "";
    std::string password  = params.get("password")   ? params.get("password")   : "";

    if (email.empty() || password.empty()) {
        return crow::response(400, "Missing required fields");
    }

    RegisterUserCommand registerCommand(
        email,
        password,
        UserInformation(
            firstName.empty() ? std::nullopt : std::optional<std::string>{firstName},
            lastName.empty()  ? std::nullopt : std::optional<std::string>{lastName},
            std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt
        )
    );
    try {
        UserProfileDto registeredUser = registerUser_.execute(registerCommand);
        auto& ctx = app.get_context<crow::CookieParser>(req);
        ctx.set_cookie("userId", jwt_.generate(registeredUser.getId()))
            .path("/")
            .max_age(60 * 60 * 24 * 7);
        crow::response res(302);
        res.add_header("Location", "/user/settings/userInformation/edit");
        return res;
    } catch (const std::exception&) {
        return crow::response(400, "User registration failed");
    }
}

crow::response AuthController::postAuthLogout(const crow::request& req, RoboDadApp& app) {
    auto& ctx = app.get_context<crow::CookieParser>(req);
    ctx.set_cookie("userId", "")
        .path("/")
        .max_age(0);
    crow::response res(302);
    res.add_header("Location", "/auth/login");
    return res;
}

crow::response AuthController::getAuthGoogle(const crow::request& req) {
    if (!googleOAuth_.isConfigured()) {
        return crow::response(503, "Google OAuth is not configured");
    }

    crow::response res(302);
    res.set_header("Location", googleOAuth_.buildAuthorizationUrl());
    return res;
}

crow::response AuthController::getAuthGoogleCallback(const crow::request& req, RoboDadApp& app) {
    const std::string error = req.url_params.get("error") ? req.url_params.get("error") : "";
    if (!error.empty()) {
        crow::response res(302);
        res.set_header("Location", "/auth/login");
        return res;
    }

    const std::string code = req.url_params.get("code") ? req.url_params.get("code") : "";
    if (code.empty()) {
        return crow::response(400, "Missing authorization code");
    }

    try {
        const std::string accessToken = googleOAuth_.exchangeCodeForToken(code);
        const GoogleUserInfo profile  = googleOAuth_.fetchUserInfo(accessToken);

        OAuthLoginCommand cmd{ profile.email, profile.firstName, profile.lastName };
        UserProfileDto user = loginOrRegisterOAuthUser_.execute(cmd);

        auto& ctx = app.get_context<crow::CookieParser>(req);
        ctx.set_cookie("userId", jwt_.generate(user.getId()))
            .path("/")
            .max_age(60 * 60 * 24 * 7);

        crow::response res(302);
        res.set_header("Location", "/user/dashboard");
        return res;
    } catch (const std::exception&) {
        crow::response res(302);
        res.set_header("Location", "/auth/login");
        return res;
    }
}
