#pragma once

#include <crow.h>
#include "IUserRepository.h"
#include "infrastructureServices/security/IJwtService.h"
#include "infrastructureServices/security/IPasswordHasher.h"

class GoogleAuthController {
public:
    GoogleAuthController(IUserRepository& repo, IJwtService& jwt,
        IPasswordHasher& hasher,
        const std::string& clientId,
        const std::string& clientSecret)
        : repo_(repo), jwt_(jwt), hasher_(hasher),
        clientId_(clientId), clientSecret_(clientSecret) {
    }

    void registerRoutes(crow::SimpleApp& app);

private:
    IUserRepository& repo_;
    IJwtService& jwt_;
    IPasswordHasher& hasher_;
    std::string       clientId_;
    std::string       clientSecret_;

    std::string exchangeCodeForToken(const std::string& code);
    std::pair<std::string, std::string> fetchGoogleUserInfo(const std::string& accessToken);
    std::string urlEncode(const std::string& value);
};