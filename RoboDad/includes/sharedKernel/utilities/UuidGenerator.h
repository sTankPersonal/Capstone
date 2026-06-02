#pragma once

#include <string>

class UuidGenerator {
public:
    // Generates a random UUID v4 string: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    static std::string generate();
};
