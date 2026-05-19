#pragma once
#include "infrastructureServices/security/IPasswordHasher.h"

class PasswordHasher : public IPasswordHasher {
public:
    std::string hash(const std::string& password) override;
    bool verify(const std::string& password, const std::string& stored) override;
};
