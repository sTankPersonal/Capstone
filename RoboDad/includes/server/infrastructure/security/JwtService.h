#pragma once
#include "infrastructureServices/security/IJwtService.h"
#include <string>

class JwtService : public IJwtService {
    std::string secret_;
public:
    explicit JwtService(const std::string& secret) : secret_(secret) {}

    std::string generate(const std::string& userId) override;
    std::optional<std::string> verify(const std::string& token) override;
};
