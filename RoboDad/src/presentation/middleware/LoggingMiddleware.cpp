#include "presentation/middleware/LoggingMiddleware.h"

void LoggingMiddleware::before_handle(crow::request& req, crow::response&, context&) {
    CROW_LOG_INFO << "[Request] " << crow::method_name(req.method) << " " << req.url;
}

void LoggingMiddleware::after_handle(crow::request&, crow::response& res, context&) {
    if (res.code >= 500) {
        CROW_LOG_ERROR << "[Response] " << res.code;
    } else if (res.code >= 400) {
        CROW_LOG_WARNING << "[Response] " << res.code;
    }
}
