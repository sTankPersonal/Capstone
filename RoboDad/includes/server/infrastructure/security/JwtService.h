#pragma once
#include <cstdint>
#include <optional>
#include <string>

class JwtService {
    std::string secret_;
public:
    explicit JwtService(const std::string& secret) : secret_(secret) {}

    std::string generate(uint32_t userId);
    std::optional<uint32_t> verify(const std::string& token);
};
