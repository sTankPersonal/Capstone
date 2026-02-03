#include <gtest/gtest.h>
#include "User.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include "UserId.h"
#include "CountryId.h"
#include "CurrencyId.h"
#include "LanguageId.h"
#include "EmploymentStatusId.h"
#include <chrono>

static std::chrono::year_month_day makeDate(int y, int m, int d) {
    return {std::chrono::year{y}, std::chrono::month{static_cast<unsigned>(m)}, std::chrono::day{static_cast<unsigned>(d)}};
}

// ── UserLogin ─────────────────────────────────────────────────────────────────

TEST(UserLoginTest, ConstructorPreservesEmail) {
    UserLogin login("alice@example.com", std::nullopt);
    EXPECT_EQ(login.getEmail(), "alice@example.com");
}

TEST(UserLoginTest, PasswordHashPreservedWhenProvided) {
    UserLogin login("alice@example.com", std::optional<std::string>{"hashed_secret"});
    ASSERT_TRUE(login.getPasswordHash().has_value());
    EXPECT_EQ(login.getPasswordHash().value(), "hashed_secret");
}

TEST(UserLoginTest, PasswordHashEmptyWhenNotProvided) {
    UserLogin login("bob@example.com", std::nullopt);
    EXPECT_FALSE(login.getPasswordHash().has_value());
}

// ── UserInformation ───────────────────────────────────────────────────────────

TEST(UserInformationTest, ConstructorPreservesAllFieldsWhenSet) {
    auto dob = makeDate(1990, 5, 15);
    UserInformation info(
        "Alice", "Smith", dob,
        CountryId{"CA"}, CurrencyId{"CAD"}, LanguageId{"en"}, EmploymentStatusId{"employed"}
    );
    ASSERT_TRUE(info.getFirstName().has_value());
    EXPECT_EQ(info.getFirstName().value(), "Alice");
    ASSERT_TRUE(info.getLastName().has_value());
    EXPECT_EQ(info.getLastName().value(), "Smith");
    ASSERT_TRUE(info.getDateOfBirth().has_value());
    EXPECT_EQ(info.getDateOfBirth().value(), dob);
    ASSERT_TRUE(info.getCountryId().has_value());
    EXPECT_EQ(info.getCountryId().value().getId(), "CA");
    ASSERT_TRUE(info.getCurrencyId().has_value());
    EXPECT_EQ(info.getCurrencyId().value().getId(), "CAD");
    ASSERT_TRUE(info.getLanguageId().has_value());
    EXPECT_EQ(info.getLanguageId().value().getId(), "en");
    ASSERT_TRUE(info.getEmploymentStatusId().has_value());
    EXPECT_EQ(info.getEmploymentStatusId().value().getId(), "employed");
}

TEST(UserInformationTest, DefaultConstructorYieldsAllEmptyOptionals) {
    UserInformation info;
    EXPECT_FALSE(info.getFirstName().has_value());
    EXPECT_FALSE(info.getLastName().has_value());
    EXPECT_FALSE(info.getDateOfBirth().has_value());
    EXPECT_FALSE(info.getCountryId().has_value());
    EXPECT_FALSE(info.getCurrencyId().has_value());
    EXPECT_FALSE(info.getLanguageId().has_value());
    EXPECT_FALSE(info.getEmploymentStatusId().has_value());
}

TEST(UserInformationTest, PartialFieldsAllowed) {
    UserInformation info("Bob", std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    ASSERT_TRUE(info.getFirstName().has_value());
    EXPECT_EQ(info.getFirstName().value(), "Bob");
    EXPECT_FALSE(info.getLastName().has_value());
}

// ── User ──────────────────────────────────────────────────────────────────────

TEST(UserTest, ConstructorPreservesUserId) {
    auto date = makeDate(2024, 1, 1);
    User user(UserId{"user-1"}, UserLogin("alice@example.com", std::nullopt), UserInformation{}, date, date);
    EXPECT_EQ(user.getId().getId(), "user-1");
}

TEST(UserTest, ConstructorPreservesUserLogin) {
    auto date = makeDate(2024, 1, 1);
    UserLogin login("alice@example.com", std::optional<std::string>{"hash"});
    User user(UserId{"user-2"}, login, UserInformation{}, date, date);
    EXPECT_EQ(user.getUserLogin().getEmail(), "alice@example.com");
    ASSERT_TRUE(user.getUserLogin().getPasswordHash().has_value());
    EXPECT_EQ(user.getUserLogin().getPasswordHash().value(), "hash");
}

TEST(UserTest, ConstructorPreservesUserInformation) {
    auto date = makeDate(2024, 6, 15);
    UserInformation info("Bob", "Jones", std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    User user(UserId{"user-3"}, UserLogin("bob@example.com", std::nullopt), info, date, date);
    ASSERT_TRUE(user.getUserInformation().getFirstName().has_value());
    EXPECT_EQ(user.getUserInformation().getFirstName().value(), "Bob");
    ASSERT_TRUE(user.getUserInformation().getLastName().has_value());
    EXPECT_EQ(user.getUserInformation().getLastName().value(), "Jones");
}

TEST(UserTest, ConstructorPreservesTimestamps) {
    auto created = makeDate(2024, 1, 1);
    auto updated = makeDate(2024, 6, 30);
    User user(UserId{"user-4"}, UserLogin("carol@example.com", std::nullopt), UserInformation{}, created, updated);
    EXPECT_EQ(user.getCreatedAt(), created);
    EXPECT_EQ(user.getUpdatedAt(), updated);
}
