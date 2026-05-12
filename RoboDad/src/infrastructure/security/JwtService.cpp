#include "infrastructure/security/JwtService.h"
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <cstdint>
#include <stdexcept>

namespace {

std::string base64urlEncode(const unsigned char* data, size_t len) {
    static const char* table =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(((len + 2) / 3) * 4);
    for (size_t i = 0; i < len; i += 3) {
        const uint32_t v =
            (static_cast<uint32_t>(data[i]) << 16) |
            (i + 1 < len ? static_cast<uint32_t>(data[i + 1]) << 8 : 0u) |
            (i + 2 < len ? static_cast<uint32_t>(data[i + 2])      : 0u);
        out.push_back(table[(v >> 18) & 0x3f]);
        out.push_back(table[(v >> 12) & 0x3f]);
        out.push_back(i + 1 < len ? table[(v >> 6) & 0x3f] : '=');
        out.push_back(i + 2 < len ? table[(v     ) & 0x3f] : '=');
    }
    for (char& c : out) {
        if      (c == '+') c = '-';
        else if (c == '/') c = '_';
    }
    while (!out.empty() && out.back() == '=') out.pop_back();
    return out;
}

std::string base64urlEncodeStr(const std::string& s) {
    return base64urlEncode(reinterpret_cast<const unsigned char*>(s.data()), s.size());
}

std::string base64urlDecode(const std::string& s) {
    std::string b64 = s;
    for (char& c : b64) {
        if      (c == '-') c = '+';
        else if (c == '_') c = '/';
    }
    while (b64.size() % 4) b64 += '=';

    BIO* b64bio = BIO_new(BIO_f_base64());
    BIO_set_flags(b64bio, BIO_FLAGS_BASE64_NO_NL);
    BIO* mem = BIO_new_mem_buf(b64.data(), static_cast<int>(b64.size()));
    BIO_push(b64bio, mem);

    std::string result(b64.size(), '\0');
    const int len = BIO_read(b64bio, result.data(), static_cast<int>(result.size()));
    BIO_free_all(b64bio);

    if (len < 0) return "";
    result.resize(static_cast<size_t>(len));
    return result;
}

std::string hmacSha256(const std::string& key, const std::string& data) {
    unsigned char digest[32];
    unsigned int  digestLen = 32;
    HMAC(EVP_sha256(),
         key.data(),  static_cast<int>(key.size()),
         reinterpret_cast<const unsigned char*>(data.data()), data.size(),
         digest, &digestLen);
    return base64urlEncode(digest, digestLen);
}

} // namespace

std::string JwtService::generate(uint32_t userId) {
    const std::string header  = base64urlEncodeStr(R"({"alg":"HS256","typ":"JWT"})");
    const std::string payload = base64urlEncodeStr(R"({"sub":")" + std::to_string(userId) + R"("})");
    const std::string toSign  = header + "." + payload;
    return toSign + "." + hmacSha256(secret_, toSign);
}

std::optional<uint32_t> JwtService::verify(const std::string& token) {
    const auto dot1 = token.find('.');
    if (dot1 == std::string::npos) return std::nullopt;
    const auto dot2 = token.find('.', dot1 + 1);
    if (dot2 == std::string::npos) return std::nullopt;

    const std::string toSign = token.substr(0, dot2);
    const std::string sig    = token.substr(dot2 + 1);
    if (hmacSha256(secret_, toSign) != sig) return std::nullopt;

    const std::string payloadJson = base64urlDecode(token.substr(dot1 + 1, dot2 - dot1 - 1));
    auto subStart = payloadJson.find(R"("sub":")");
    if (subStart == std::string::npos) return std::nullopt;
    subStart += 7;
    const auto subEnd = payloadJson.find('"', subStart);
    if (subEnd == std::string::npos) return std::nullopt;

    try {
        return static_cast<uint32_t>(std::stoul(payloadJson.substr(subStart, subEnd - subStart)));
    } catch (...) {
        return std::nullopt;
    }
}
