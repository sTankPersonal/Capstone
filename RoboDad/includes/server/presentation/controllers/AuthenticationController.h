#pragma once

#include <crow.h>
#include "application/auth/LoginUser.h"
#include "application/auth/RegisterUser.h"
#include "IUserRepository.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"

class AuthenticationController {
    LoginUser    loginUser_;
    RegisterUser registerUser_;
    IJwtService& jwt_;

    std::optional<std::string> extractUserId(const crow::request& req) const;

public:
    AuthenticationController(IUserRepository& repo, IPasswordHasher& hasher, IJwtService& jwt)
        : loginUser_(repo, hasher), registerUser_(repo, hasher), jwt_(jwt) {}

    void registerRoutes(crow::SimpleApp& app);
};
