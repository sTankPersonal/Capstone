#include <gtest/gtest.h>
#include "infrastructure/security/JwtService.h"

TEST(JwtServiceTest, GenerateProducesThreePartToken) {
    JwtService svc("supersecret");
    const std::string token = svc.generate("user-42");
    int dots = 0;
    for (char c : token) dots += (c == '.');
    EXPECT_EQ(dots, 2);
}

TEST(JwtServiceTest, VerifyReturnsCorrectUserIdForValidToken) {
    JwtService svc("supersecret");
    const std::string token = svc.generate("user-42");
    const auto result = svc.verify(token);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "user-42");
}

TEST(JwtServiceTest, VerifyRoundTripsMultipleUserIds) {
    JwtService svc("supersecret");
    EXPECT_EQ(*svc.verify(svc.generate("user-1")),     "user-1");
    EXPECT_EQ(*svc.verify(svc.generate("user-100")),   "user-100");
    EXPECT_EQ(*svc.verify(svc.generate("uuid-abc123")), "uuid-abc123");
}

TEST(JwtServiceTest, VerifyReturnsFalseForTamperedSignature) {
    JwtService svc("supersecret");
    std::string token = svc.generate("user-1");
    auto& c = token[token.size() - 2];
    c = (c == 'A') ? 'B' : 'A';
    EXPECT_FALSE(svc.verify(token).has_value());
}

TEST(JwtServiceTest, VerifyReturnsFalseForDifferentSecret) {
    JwtService svc1("secret1");
    JwtService svc2("secret2");
    EXPECT_FALSE(svc2.verify(svc1.generate("user-1")).has_value());
}

TEST(JwtServiceTest, VerifyReturnsFalseForMalformedToken) {
    JwtService svc("supersecret");
    EXPECT_FALSE(svc.verify("").has_value());
    EXPECT_FALSE(svc.verify("nodots").has_value());
    EXPECT_FALSE(svc.verify("only.one.dot").has_value());
}
