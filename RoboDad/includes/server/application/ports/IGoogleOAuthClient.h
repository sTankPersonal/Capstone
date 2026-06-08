#pragma once
#include <string>

struct GoogleUserInfo {
    std::string googleId;
    std::string email;
    std::string firstName;
    std::string lastName;
};

class IGoogleOAuthClient {
public:
    virtual ~IGoogleOAuthClient() = default;

    virtual bool isConfigured() const = 0;

    // Build the Google authorization URL to redirect the browser to.
    virtual std::string buildAuthorizationUrl() const = 0;

    // Exchange an authorization code for a Google access token.
    virtual std::string exchangeCodeForToken(const std::string& code) = 0;

    // Fetch the authenticated user's profile using an access token.
    virtual GoogleUserInfo fetchUserInfo(const std::string& accessToken) = 0;
};
