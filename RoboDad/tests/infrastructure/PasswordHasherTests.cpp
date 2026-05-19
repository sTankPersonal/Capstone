#include <gtest/gtest.h>
#include "infrastructure/security/PasswordHasher.h"

TEST(PasswordHasherTest, HashProducesStoredStringWithSaltSeparator) {
    PasswordHasher hasher;
    std::string stored = hasher.hash("secret");
    EXPECT_FALSE(stored.empty());
    EXPECT_NE(stored.find(':'), std::string::npos);
}

TEST(PasswordHasherTest, VerifyReturnsTrueForCorrectPassword) {
    PasswordHasher hasher;
    std::string stored = hasher.hash("mypassword");
    EXPECT_TRUE(hasher.verify("mypassword", stored));
}

TEST(PasswordHasherTest, VerifyReturnsFalseForWrongPassword) {
    PasswordHasher hasher;
    std::string stored = hasher.hash("correct");
    EXPECT_FALSE(hasher.verify("wrong", stored));
}

TEST(PasswordHasherTest, TwoHashesOfSamePasswordDifferDueToSalt) {
    PasswordHasher hasher;
    std::string a = hasher.hash("password");
    std::string b = hasher.hash("password");
    EXPECT_NE(a, b);
}

TEST(PasswordHasherTest, VerifyReturnsFalseForMalformedStoredString) {
    PasswordHasher hasher;
    EXPECT_FALSE(hasher.verify("password", "nocolon"));
    EXPECT_FALSE(hasher.verify("password", ""));
}
