#pragma once

#include <optional>
#include <string>

class IJwtService {
public:
    virtual ~IJwtService() = default;
    virtual std::string generate(const std::string& userId, const std::string& role = "user") = 0;
    virtual std::optional<std::string> verify(const std::string& token) = 0;
    virtual std::optional<std::string> verifyRole(const std::string& token) = 0;
};
