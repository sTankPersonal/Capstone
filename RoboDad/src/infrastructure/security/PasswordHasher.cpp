#include "infrastructure/security/PasswordHasher.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {

std::string toHex(const unsigned char* data, size_t len) {
    std::ostringstream oss;
    for (size_t i = 0; i < len; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(data[i]);
    return oss.str();
}

std::string sha256hex(const std::string& input) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
    EVP_DigestUpdate(ctx, input.data(), input.size());
    unsigned char digest[EVP_MAX_MD_SIZE];
    unsigned int len = 0;
    EVP_DigestFinal_ex(ctx, digest, &len);
    EVP_MD_CTX_free(ctx);
    return toHex(digest, len);
}

} // namespace

std::string PasswordHasher::hash(const std::string& password) {
    unsigned char saltBytes[16];
    if (RAND_bytes(saltBytes, sizeof(saltBytes)) != 1)
        throw std::runtime_error("RAND_bytes failed");
    std::string salt = toHex(saltBytes, sizeof(saltBytes));
    return salt + ":" + sha256hex(salt + password);
}

bool PasswordHasher::verify(const std::string& password, const std::string& stored) {
    const auto colon = stored.find(':');
    if (colon == std::string::npos) return false;
    const std::string salt     = stored.substr(0, colon);
    const std::string expected = stored.substr(colon + 1);
    return sha256hex(salt + password) == expected;
}
