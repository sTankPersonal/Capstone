#pragma once
#include "sharedKernel/presentation/IController.h"
#include "domain/valueObjects/ids/UserId.h"

#include "application/users/services/GetUserProfile.h"
#include "application/users/services/UpdateUserProfile.h"
#include "application/users/services/UpdateUserPassword.h"
#include "application/users/services/DeleteUser.h"

#include "presentation/AppType.h"

/*
Routes:
    GET: /user/dashboard - Loads the user dashboard page
    GET: /user/settings - Loads the user settings page
    GET: /user/settings/userLogin - Loads the user login information page
    GET: /user/settings/userInformation - Loads the user information page

    GET: /user/settings/userLogin/edit - Loads the user login information edit page
        - Email
        - Password
    GET: /user/settings/userInformation/edit - Loads the user information edit page
        - First Name
        - Last Name
        - Date of Birth
        - Country
        - Currency
        - Language
        - Employment Status
    GET: /user/settings/delete - Loads the user deletion confirmation page

    POST: /user/settings/userLogin/edit - Submits the user login information edit form
    POST: /user/settings/userInformation/edit - Submits the user information edit form
    POST: /user/settings/delete - Submits the user deletion confirmation form
*/

class UserController : public IController<RoboDadApp> {
    GetUserProfile getUserProfile_;
    UpdateUserProfile updateUserProfile_;
    UpdateUserPassword updateUserPassword_;
    DeleteUser deleteUser_;
public:
    UserController(const GetUserProfile& getUserProfile, const UpdateUserProfile& updateUserProfile, const UpdateUserPassword& updateUserPassword, const DeleteUser& deleteUser);
    void registerRoutes(RoboDadApp& app) override;

    crow::response getUserDashboardPage(const crow::request& req, UserId user_id);
    crow::response getUserSettingsPage(const crow::request& req, UserId user_id);
    crow::response getUserSettingsLoginPage(const crow::request& req, UserId user_id);
    crow::response getUserSettingsInformationPage(const crow::request& req, UserId user_id);

    crow::response getEditUserSettingsLoginPage(const crow::request& req, UserId user_id);
    crow::response getEditUserSettingsInformationPage(const crow::request& req, UserId user_id);
    crow::response getDeleteUserSettingsPage(const crow::request& req, UserId user_id);

    crow::response postEditUserSettingsLogin(const crow::request& req, UserId user_id);
    crow::response postEditUserSettingsInformation(const crow::request& req, UserId user_id);
    crow::response postDeleteUserSettings(const crow::request& req, UserId user_id, RoboDadApp& app);
};