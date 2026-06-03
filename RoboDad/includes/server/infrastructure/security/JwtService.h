#pragma once
#include "IJwtService.h"
#include <string>

class JwtService : public IJwtService {
    std::string secret_;
public:
    explicit JwtService(const std::string& secret) : secret_(secret) {}

    std::string generate(const std::string& userId, const std::string& role = "user") override;
    std::optional<std::string> verify(const std::string& token) override;
    std::optional<std::string> verifyRole(const std::string& token) override;
};
