#pragma once
#include "infrastructure/apiClient/ApiClient.h"
#include "application/ports/IGoogleOAuthClient.h"
#include <string>

class GoogleOAuthService : public ApiClient, public IGoogleOAuthClient {
    std::string clientSecret_;
    std::string redirectUri_;
public:
    GoogleOAuthService(const std::string& clientId,
                       const std::string& clientSecret,
                       const std::string& redirectUri);

    bool isConfigured() const override { return ApiClient::isConfigured(); }

    // Build the Google OAuth authorization URL to redirect the browser to.
    std::string buildAuthorizationUrl() const override;

    // Exchange an authorization code for a Google access token.
    std::string exchangeCodeForToken(const std::string& code) override;

    // Fetch the authenticated user's profile using an access token.
    GoogleUserInfo fetchUserInfo(const std::string& accessToken) override;
};
