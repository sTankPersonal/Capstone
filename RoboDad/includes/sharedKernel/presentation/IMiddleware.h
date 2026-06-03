#pragma once

#include <crow.h>

class IMiddleware {
    virtual void before_handle(crow::request& req, crow::response& res, crow::context& ctx) = 0;
    virtual void after_handle(crow::request& req, crow::response& res, crow::context& ctx) = 0;
};