#pragma once

#include <crow.h>
#include <optional>
#include "crow/middlewares/cookie_parser.h"
#include "security/IJwtService.h"

struct AdminMiddleware {
    struct context {
        bool isAdmin = false;
    };

    template<typename AllContext>
    void before_handle(crow::request& req, crow::response& res, context& ctx, AllContext& all_ctx) {
        if (!ctx.isAdmin) {
            return;
        }
        auto& ctxCookie = all_ctx.template get<crow::CookieParser>();
        std::string userId = ctxCookie.get_cookie("userId");
        if (userId.empty() || !jwt_) {
            return redirectToHome(res);
        }
        std::optional<std::string> verifiedRole = jwt_->verifyRole(userId);
        if (!verifiedRole || *verifiedRole != "admin") {
            return redirectToHome(res);
        }
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx);
    void init(IJwtService& jwt);
    void redirectToHome(crow::response& res);

private:
    IJwtService* jwt_ = nullptr;
};
