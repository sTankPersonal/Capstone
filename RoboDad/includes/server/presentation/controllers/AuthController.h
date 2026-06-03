#pragma once
#include "sharedKernel/presentation/IController.h"
#include "presentation/AppType.h"

#include "application/users/services/LoginUser.h"
#include "application/users/services/RegisterUser.h"
#include "application/users/services/LoginOrRegisterOAuthUser.h"
#include "application/ports/IGoogleOAuthClient.h"
#include "security/IJwtService.h"

/*
    GET: /auth/login - Loads the login page
    GET: /auth/register - Loads the registration page
    GET: /auth/logout - Logs the user out and redirects to the login page

    POST: /auth/login - Submits the login form
    POST: /auth/register - Submits the registration form
    POST: /auth/logout - Submits the logout form

    GET: /auth/google - Initiates the Google OAuth login flow
    GET: /auth/google/callback - Handles the callback from Google OAuth login flow
*/

class AuthController : public IController<RoboDadApp> {
    LoginUser loginUser_;
    RegisterUser registerUser_;
    LoginOrRegisterOAuthUser loginOrRegisterOAuthUser_;
    IGoogleOAuthClient& googleOAuth_;
    IJwtService& jwt_;
public:
    AuthController(const LoginUser& loginUser,
                   const RegisterUser& registerUser,
                   const LoginOrRegisterOAuthUser& loginOrRegisterOAuthUser,
                   IGoogleOAuthClient& googleOAuth,
                   IJwtService& jwt);

    void registerRoutes(RoboDadApp& app) override;

    crow::response getAuthLoginPage(const crow::request& req);
    crow::response getAuthRegisterPage(const crow::request& req);
    crow::response getAuthLogout(const crow::request& req);

    crow::response postAuthLogin(const crow::request& req, RoboDadApp& app);
    crow::response postAuthRegister(const crow::request& req, RoboDadApp& app);
    crow::response postAuthLogout(const crow::request& req, RoboDadApp& app);

    crow::response getAuthGoogle(const crow::request& req);
    crow::response getAuthGoogleCallback(const crow::request& req, RoboDadApp& app);
};