#pragma once

#include <crow.h>
#include "crow/middlewares/cookie_parser.h"
#include "presentation/middleware/AuthMiddleware.h"
#include "presentation/middleware/AdminMiddleware.h"

using RoboDadApp = crow::App<crow::CookieParser, AuthMiddleware, AdminMiddleware>;
