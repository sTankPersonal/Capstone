#include <gtest/gtest.h>
#include "infrastructure/security/JwtService.h"

TEST(JwtServiceTest, GenerateProducesThreePartToken) {
    JwtService svc("supersecret");
    const std::string token = svc.generate(42u);
    int dots = 0;
    for (char c : token) dots += (c == '.');
    EXPECT_EQ(dots, 2);
}

TEST(JwtServiceTest, VerifyReturnsCorrectUserIdForValidToken) {
    JwtService svc("supersecret");
    const std::string token = svc.generate(42u);
    const auto result = svc.verify(token);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 42u);
}

TEST(JwtServiceTest, VerifyRoundTripsMultipleUserIds) {
    JwtService svc("supersecret");
    EXPECT_EQ(*svc.verify(svc.generate(1u)),   1u);
    EXPECT_EQ(*svc.verify(svc.generate(100u)), 100u);
    EXPECT_EQ(*svc.verify(svc.generate(0u)),   0u);
}

TEST(JwtServiceTest, VerifyReturnsFalseForTamperedSignature) {
    JwtService svc("supersecret");
    std::string token = svc.generate(1u);
    token.back() = (token.back() == 'A') ? 'B' : 'A';
    EXPECT_FALSE(svc.verify(token).has_value());
}

TEST(JwtServiceTest, VerifyReturnsFalseForDifferentSecret) {
    JwtService svc1("secret1");
    JwtService svc2("secret2");
    EXPECT_FALSE(svc2.verify(svc1.generate(1u)).has_value());
}

TEST(JwtServiceTest, VerifyReturnsFalseForMalformedToken) {
    JwtService svc("supersecret");
    EXPECT_FALSE(svc.verify("").has_value());
    EXPECT_FALSE(svc.verify("nodots").has_value());
    EXPECT_FALSE(svc.verify("only.one.dot").has_value()); // valid structure, bad sig
}
