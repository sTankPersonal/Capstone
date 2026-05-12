#include <gtest/gtest.h>
#include "infrastructure/security/PasswordHasher.h"

TEST(PasswordHasherTest, HashProducesStoredStringWithSaltSeparator) {
    std::string stored = PasswordHasher::hash("secret");
    EXPECT_FALSE(stored.empty());
    EXPECT_NE(stored.find(':'), std::string::npos);
}

TEST(PasswordHasherTest, VerifyReturnsTrueForCorrectPassword) {
    std::string stored = PasswordHasher::hash("mypassword");
    EXPECT_TRUE(PasswordHasher::verify("mypassword", stored));
}

TEST(PasswordHasherTest, VerifyReturnsFalseForWrongPassword) {
    std::string stored = PasswordHasher::hash("correct");
    EXPECT_FALSE(PasswordHasher::verify("wrong", stored));
}

TEST(PasswordHasherTest, TwoHashesOfSamePasswordDifferDueToSalt) {
    std::string a = PasswordHasher::hash("password");
    std::string b = PasswordHasher::hash("password");
    EXPECT_NE(a, b);
}

TEST(PasswordHasherTest, VerifyReturnsFalseForMalformedStoredString) {
    EXPECT_FALSE(PasswordHasher::verify("password", "nocolon"));
    EXPECT_FALSE(PasswordHasher::verify("password", ""));
}
