#include "infrastructure/security/JwtService.h"
#include <jwt-cpp/jwt.h>
#include <chrono>
#include <stdexcept>

std::string JwtService::generate(const std::string& userId, const std::string& role) {
    return jwt::create()
        .set_issuer("robodad")
        .set_subject(userId)
        .set_payload_claim("role", jwt::claim(role))
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(24))
        .sign(jwt::algorithm::hs256{secret_});
}

std::optional<std::string> JwtService::verify(const std::string& token) {
    try {
        auto decoded = jwt::decode(token);

        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret_})
            .with_issuer("robodad")
            .verify(decoded);

        return decoded.get_subject();
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::optional<std::string> JwtService::verifyRole(const std::string& token) {
    try {
        auto decoded = jwt::decode(token);

        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret_})
            .with_issuer("robodad")
            .verify(decoded);

        return decoded.get_payload_claim("role").as_string();
    } catch (const std::exception&) {
        return std::nullopt;
    }
}
