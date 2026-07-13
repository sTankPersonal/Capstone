#include "presentation/controllers/AuthController.h"
#include "application/users/dtos/UserProfileDto.h"
#include "application/users/commands/LoginUserCommand.h"
#include "application/users/commands/RegisterUserCommand.h"
#include "application/users/commands/OAuthLoginCommand.h"
#include "application/users/commands/VerifyEmailCommand.h"
#include "application/users/commands/ResendVerificationCommand.h"
#include "domain/valueObjects/UserInformation.h"

#include <crow/mustache.h>
#include <crow/query_string.h>


AuthController::AuthController(const LoginUser& loginUser,
                               const RegisterUser& registerUser,
                               const LoginOrRegisterOAuthUser& loginOrRegisterOAuthUser,
                               const VerifyEmail& verifyEmail,
                               const ResendVerificationEmail& resendVerificationEmail,
                               IGoogleOAuthClient& googleOAuth,
                               IJwtService& jwt)
    : loginUser_(loginUser)
    , registerUser_(registerUser)
    , loginOrRegisterOAuthUser_(loginOrRegisterOAuthUser)
    , verifyEmail_(verifyEmail)
    , resendVerificationEmail_(resendVerificationEmail)
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

    CROW_ROUTE(app, "/auth/check-email")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthCheckEmailPage(req);
        });

    CROW_ROUTE(app, "/auth/verify")
        .methods(crow::HTTPMethod::GET)([this](const crow::request& req){
            return getAuthVerify(req);
        });

    CROW_ROUTE(app, "/auth/resend-verification")
        .methods(crow::HTTPMethod::POST)([this](const crow::request& req){
            return postAuthResendVerification(req);
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

crow::response AuthController::getAuthCheckEmailPage(const crow::request& req) {
    return crow::response(crow::mustache::load("check-email.html").render());
}

crow::response AuthController::getAuthVerify(const crow::request& req) {
    const std::string token = req.url_params.get("token") ? req.url_params.get("token") : "";
    if (token.empty()) {
        return crow::response(400, "Missing verification token");
    }

    VerifyEmailCommand cmd{token};
    VerifyEmailResult result = verifyEmail_.execute(cmd);

    crow::response res(302);
    if (result == VerifyEmailResult::Verified) {
        res.add_header("Location", "/auth/login?verified=1");
    } else {
        res.add_header("Location", "/auth/check-email?expired=1");
    }
    return res;
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
    if (!user->isVerified()) {
        crow::response res(302);
        res.add_header("Location", "/auth/login?error=unverified&email=" + email);
        return res;
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
        registerUser_.execute(registerCommand);
        crow::response res(302);
        res.add_header("Location", "/auth/check-email?email=" + email);
        return res;
    } catch (const std::exception&) {
        return crow::response(400, "User registration failed");
    }
}

crow::response AuthController::postAuthResendVerification(const crow::request& req) {
    crow::query_string params("?" + req.body);
    std::string email = params.get("email") ? params.get("email") : "";

    if (!email.empty()) {
        ResendVerificationCommand cmd{email};
        resendVerificationEmail_.execute(cmd);
    }

    crow::response res(302);
    res.add_header("Location", "/auth/check-email?email=" + email + "&sent=1");
    return res;
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
        bool isNewUser = false;
        UserProfileDto user = loginOrRegisterOAuthUser_.execute(cmd, isNewUser);

        auto& ctx = app.get_context<crow::CookieParser>(req);
        ctx.set_cookie("userId", jwt_.generate(user.getId()))
            .path("/")
            .max_age(60 * 60 * 24 * 7);

        // First-time OAuth users are prompted to complete the rest of their
        // profile, mirroring the traditional registration flow.
        crow::response res(302);
        res.set_header("Location", isNewUser ? "/user/settings/userInformation/edit"
                                             : "/user/dashboard");
        return res;
    } catch (const std::exception&) {
        crow::response res(302);
        res.set_header("Location", "/auth/login");
        return res;
    }
}
