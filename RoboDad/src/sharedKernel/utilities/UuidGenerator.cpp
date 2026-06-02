#include "UuidGenerator.h"
#include <random>
#include <sstream>
#include <iomanip>
#include <array>
#include <cstdint>
#include <cstring>

std::string UuidGenerator::generate() {
    static thread_local std::mt19937 rng{std::random_device{}()};
    std::uniform_int_distribution<uint32_t> dist(0, 0xFFFFFFFF);

    std::array<uint8_t, 16> bytes{};
    for (int i = 0; i < 4; ++i) {
        uint32_t val = dist(rng);
        std::memcpy(bytes.data() + i * 4, &val, 4);
    }

    // Version 4
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    // Variant bits (10xx)
    bytes[8] = (bytes[8] & 0x3F) | 0x80;

    std::ostringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 16; ++i) {
        ss << std::setw(2) << static_cast<unsigned>(bytes[i]);
        if (i == 3 || i == 5 || i == 7 || i == 9) ss << '-';
    }
    return ss.str();
}
