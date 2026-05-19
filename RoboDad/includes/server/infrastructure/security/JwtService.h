#pragma once
#include "infrastructureServices/security/IJwtService.h"
#include <string>

class JwtService : public IJwtService {
    std::string secret_;
public:
    explicit JwtService(const std::string& secret) : secret_(secret) {}

    std::string generate(uint32_t userId) override;
    std::optional<uint32_t> verify(const std::string& token) override;
};
