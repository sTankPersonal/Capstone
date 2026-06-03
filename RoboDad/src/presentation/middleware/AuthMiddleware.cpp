#include "presentation/middleware/AuthMiddleware.h"

void AuthMiddleware::after_handle(crow::request&, crow::response&, context&) {}

void AuthMiddleware::init(IJwtService& jwt) {
    jwt_ = &jwt;
}

void AuthMiddleware::redirectToHome(crow::response& res) {
    res.redirect("/auth/login");
    res.end();
}
