#pragma once

#include <crow.h>
#include <string>
#include <optional>
#include "crow/middlewares/cookie_parser.h"
#include "security/IJwtService.h"

struct AuthMiddleware {
    struct context {
        bool isPublic = false;
        std::string userId;
    };

    template<typename AllContext>
    void before_handle(crow::request& req, crow::response& res, context& ctx, AllContext& all_ctx) {
        if (ctx.isPublic || req.url.starts_with("/auth/")) {
            ctx.isPublic = true;
            return;
        }
        auto& ctxCookie = all_ctx.template get<crow::CookieParser>();
        std::string userId = ctxCookie.get_cookie("userId");
        if (userId.empty() || !jwt_) {
            return redirectToHome(res);
        }
        std::optional<std::string> verifiedUserId = jwt_->verify(userId);
        if (!verifiedUserId) {
            return redirectToHome(res);
        }
        ctx.userId = *verifiedUserId;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx);
    void init(IJwtService& jwt);
    void redirectToHome(crow::response& res);

private:
    IJwtService* jwt_ = nullptr;
};
