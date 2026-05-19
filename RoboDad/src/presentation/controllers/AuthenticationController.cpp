#include "AuthenticationController.h"
#include "domain/user/EmploymentStatus.h"
#include <crow.h>
#include <stdexcept>
#include <string>

void AuthenticationController::registerRoutes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/login")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
            });
    CROW_ROUTE(app, "/signup")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/signup.html");
        res.end();
            });
    CROW_ROUTE(app, "/logout")
        ([](const crow::request&, crow::response& res) {
        res.set_static_file_info("public/login.html");
        res.end();
            });

    CROW_ROUTE(app, "/api/auth/login").methods("POST"_method)
    ([this](const crow::request& req) {
        try {
            const std::string bodyStr = "?" + req.body;
            crow::query_string form(bodyStr);
            std::string email    = form.get("email")    ? form.get("email")    : "";
            std::string password = form.get("password") ? form.get("password") : "";

            auto user = loginUser_.execute(email, password);
            if (!user) {
                crow::response res(302);
                res.set_header("Location", "/login?error=invalid");
                return res;
            }

            crow::response res(302);
            res.set_header("Set-Cookie", "token=" + jwt_.generate(user->getId()) + "; HttpOnly; Path=/");
            res.set_header("Location", "/chat");
            return res;
        } catch (const std::exception&) {
            crow::response res(302);
            res.set_header("Location", "/login?error=failed");
            return res;
        }
    });

    CROW_ROUTE(app, "/api/auth/logout").methods("POST"_method)
    ([]() {
        return crow::response(200, R"({"message":"Logged out"})");
    });

    CROW_ROUTE(app, "/api/auth/register").methods("POST"_method)
    ([this](const crow::request& req) {
        try {
            const std::string bodyStr = "?" + req.body;
            crow::query_string form(bodyStr);
            std::string name     = form.get("name")              ? form.get("name")              : "";
            std::string email    = form.get("email")             ? form.get("email")             : "";
            std::string password = form.get("password")          ? form.get("password")          : "";
            int age              = form.get("age")               ? std::stoi(form.get("age"))    : 0;
            int statusInt        = form.get("employment_status") ? std::stoi(form.get("employment_status")) : 0;

            PersonalInfo info(name, static_cast<uint8_t>(age), static_cast<EmploymentStatus>(statusInt));
            auto user = registerUser_.execute(info, email, password);

            crow::response res(302);
            res.set_header("Set-Cookie", "token=" + jwt_.generate(user.getId()) + "; HttpOnly; Path=/");
            res.set_header("Location", "/chat");
            return res;
        } catch (const std::exception& e) {
            std::cerr << "Registration error: " << e.what() << std::endl;
            std::string msg = e.what();
            std::string location = msg.find("duplicate key") != std::string::npos
                ? "/signup?error=email_taken"
                : "/signup?error=failed";
            crow::response res(302);
            res.set_header("Location", location);
            return res;
        }
    });
}
