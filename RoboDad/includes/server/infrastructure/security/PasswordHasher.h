#pragma once
#include <string>

class PasswordHasher {
public:
    // Returns "salt:sha256hex" where salt is a random 16-byte hex string.
    static std::string hash(const std::string& password);
    static bool verify(const std::string& password, const std::string& stored);
};
