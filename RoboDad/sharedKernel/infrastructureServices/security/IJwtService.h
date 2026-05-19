#pragma once
#include <cstdint>
#include <optional>
#include <string>

class IJwtService {
public:
    virtual ~IJwtService() = default;
    virtual std::string generate(uint32_t userId) = 0;
    virtual std::optional<uint32_t> verify(const std::string& token) = 0;
};
