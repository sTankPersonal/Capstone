#pragma once

#include <crow.h>
#include "crow/middlewares/cookie_parser.h"
#include "presentation/middleware/AuthMiddleware.h"
#include "presentation/middleware/AdminMiddleware.h"
#include "presentation/middleware/LoggingMiddleware.h"

using RoboDadApp = crow::App<crow::CookieParser, LoggingMiddleware, AuthMiddleware, AdminMiddleware>;
