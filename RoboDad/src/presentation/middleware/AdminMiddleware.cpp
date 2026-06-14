#include "presentation/middleware/AdminMiddleware.h"

void AdminMiddleware::after_handle(crow::request&, crow::response&, context&) {}

void AdminMiddleware::init(IJwtService& jwt) {
    jwt_ = &jwt;
}

void AdminMiddleware::redirectToHome(crow::response& res) {
    res.redirect("/auth/login");
    res.end();
}