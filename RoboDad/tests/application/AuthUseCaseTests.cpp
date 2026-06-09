#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "IUserRepository.h"
#include "IPasswordHasher.h"
#include "LoginUser.h"
#include "RegisterUser.h"
#include "RegisterUserCommand.h"
#include "LoginUserCommand.h"
#include "UserProfileDto.h"
#include "User.h"
#include "UserId.h"
#include "UserLogin.h"
#include "UserInformation.h"
#include <chrono>

using ::testing::Return;
using ::testing::_;

static std::chrono::year_month_day today() {
    return std::chrono::year_month_day{
        std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now())};
}

static User makeUser(const std::string& id, const std::string& email) {
    auto d = today();
    return User(UserId{id}, UserLogin(email, std::nullopt), UserInformation{}, d, d);
}

class MockUserRepository : public IUserRepository {
public:
    MOCK_METHOD(User,                          create,            (const User&),        (override));
    MOCK_METHOD(std::optional<User>,           findById,          (UserId),             (override));
    MOCK_METHOD(std::vector<User>,             findAll,           (),                   (override));
    MOCK_METHOD(bool,                          update,            (const User&),        (override));
    MOCK_METHOD(bool,                          remove,            (UserId),             (override));
    MOCK_METHOD((std::optional<std::pair<UserId, std::string>>),
                                               lookupCredentials, (const std::string&), (override));
};

class MockPasswordHasher : public IPasswordHasher {
public:
    MOCK_METHOD(std::string, hash,   (const std::string&),                     (override));
    MOCK_METHOD(bool,        verify, (const std::string&, const std::string&), (override));
};

// ── RegisterUser ──────────────────────────────────────────────────────────────

TEST(RegisterUserTest, ExecuteHashesPasswordBeforeStoring) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    User expected = makeUser("uuid-1", "alice@example.com");

    EXPECT_CALL(hasher, hash("secret")).WillOnce(Return(std::string("hashed_secret")));
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    RegisterUser useCase(repo, hasher);
    UserProfileDto result = useCase.execute(RegisterUserCommand{"alice@example.com", "secret", UserInformation{}});

    EXPECT_EQ(result.getId(), "uuid-1");
}

TEST(RegisterUserTest, ExecuteReturnsCreatedUser) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    UserInformation info("Bob", "Jones", std::nullopt, std::nullopt, std::nullopt, std::nullopt, std::nullopt);
    User expected = makeUser("uuid-2", "bob@example.com");

    EXPECT_CALL(hasher, hash(_)).WillOnce(Return(std::string("any_hash")));
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    RegisterUser useCase(repo, hasher);
    UserProfileDto result = useCase.execute(RegisterUserCommand{"bob@example.com", "pass123", info});

    EXPECT_EQ(result.getId(), "uuid-2");
}

// ── LoginUser ─────────────────────────────────────────────────────────────────

TEST(LoginUserTest, ExecuteReturnsUserOnValidCredentials) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    User expected = makeUser("uuid-1", "alice@example.com");

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(UserId{"uuid-1"}, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("secret", "hashed")).WillOnce(Return(true));
    EXPECT_CALL(repo, findById(UserId{"uuid-1"})).WillOnce(Return(std::optional<User>{expected}));

    LoginUser useCase(repo, hasher);
    auto result = useCase.execute(LoginUserCommand{"alice@example.com", "secret"});

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), "uuid-1");
}

TEST(LoginUserTest, ExecuteReturnsNulloptWhenEmailNotFound) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("unknown@example.com"))
        .WillOnce(Return(std::nullopt));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute(LoginUserCommand{"unknown@example.com", "secret"}).has_value());
}

TEST(LoginUserTest, ExecuteReturnsNulloptOnWrongPassword) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(UserId{"uuid-1"}, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("wrong", "hashed")).WillOnce(Return(false));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute(LoginUserCommand{"alice@example.com", "wrong"}).has_value());
}

TEST(LoginUserTest, ExecuteReturnsNulloptWhenUserNotFoundAfterCredentialLookup) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(UserId{"uuid-1"}, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("secret", "hashed")).WillOnce(Return(true));
    EXPECT_CALL(repo, findById(UserId{"uuid-1"})).WillOnce(Return(std::nullopt));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute(LoginUserCommand{"alice@example.com", "secret"}).has_value());
}
