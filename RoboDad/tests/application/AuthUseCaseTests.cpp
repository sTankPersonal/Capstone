#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "infrastructureServices/persistance/UserRepository.h"
#include "infrastructureServices/security/IPasswordHasher.h"
#include "application/auth/LoginUser.h"
#include "application/auth/RegisterUser.h"
#include "domain/user/EmploymentStatus.h"
#include "domain/user/PersonalInfo.h"
#include "domain/user/User.h"

using ::testing::Return;
using ::testing::_;

class MockUserRepository : public UserRepository {
public:
    MOCK_METHOD(User,                  create,            (const User&, const std::string&, const std::string&), (override));
    MOCK_METHOD(std::optional<User>,   findById,          (uint32_t),                (override));
    MOCK_METHOD(std::vector<User>,     findAll,           (),                        (override));
    MOCK_METHOD(bool,                  update,            (const User&),             (override));
    MOCK_METHOD(bool,                  remove,            (uint32_t),                (override));
    MOCK_METHOD((std::optional<std::pair<uint32_t, std::string>>),
                                       lookupCredentials, (const std::string&),      (override));
};

class MockPasswordHasher : public IPasswordHasher {
public:
    MOCK_METHOD(std::string, hash,   (const std::string&),                     (override));
    MOCK_METHOD(bool,        verify, (const std::string&, const std::string&), (override));
};

// ── RegisterUser ─────────────────────────────────────────────────────────────

TEST(RegisterUserTest, ExecuteHashesPasswordBeforeStoring) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    PersonalInfo info("Alice", 30, EmploymentStatus::Employed);
    User expected(1u, info, {});

    EXPECT_CALL(hasher, hash("secret")).WillOnce(Return(std::string("hashed_secret")));
    EXPECT_CALL(repo, create(_, "alice@example.com", "hashed_secret")).WillOnce(Return(expected));

    RegisterUser useCase(repo, hasher);
    User result = useCase.execute(info, "alice@example.com", "secret");

    EXPECT_EQ(result.getId(), 1u);
}

TEST(RegisterUserTest, ExecuteReturnsCreatedUser) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    PersonalInfo info("Bob", 25, EmploymentStatus::Unemployed);
    User expected(2u, info, {});

    EXPECT_CALL(hasher, hash(_)).WillOnce(Return(std::string("any_hash")));
    EXPECT_CALL(repo, create(_, _, _)).WillOnce(Return(expected));

    RegisterUser useCase(repo, hasher);
    User result = useCase.execute(info, "bob@example.com", "pass123");

    EXPECT_EQ(result.getId(), 2u);
    EXPECT_EQ(result.getPersonalInfo().getName(), "Bob");
}

// ── LoginUser ─────────────────────────────────────────────────────────────────

TEST(LoginUserTest, ExecuteReturnsUserOnValidCredentials) {
    MockUserRepository repo;
    MockPasswordHasher hasher;
    PersonalInfo info("Alice", 30, EmploymentStatus::Employed);
    User expected(1u, info, {});

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(1u, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("secret", "hashed")).WillOnce(Return(true));
    EXPECT_CALL(repo, findById(1u)).WillOnce(Return(std::optional<User>{expected}));

    LoginUser useCase(repo, hasher);
    auto result = useCase.execute("alice@example.com", "secret");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), 1u);
    EXPECT_EQ(result->getPersonalInfo().getName(), "Alice");
}

TEST(LoginUserTest, ExecuteReturnsNulloptWhenEmailNotFound) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("unknown@example.com"))
        .WillOnce(Return(std::nullopt));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute("unknown@example.com", "secret").has_value());
}

TEST(LoginUserTest, ExecuteReturnsNulloptOnWrongPassword) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(1u, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("wrong", "hashed")).WillOnce(Return(false));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute("alice@example.com", "wrong").has_value());
}

TEST(LoginUserTest, ExecuteReturnsNulloptWhenUserNotFoundAfterCredentialLookup) {
    MockUserRepository repo;
    MockPasswordHasher hasher;

    EXPECT_CALL(repo, lookupCredentials("alice@example.com"))
        .WillOnce(Return(std::make_optional(std::make_pair(1u, std::string("hashed")))));
    EXPECT_CALL(hasher, verify("secret", "hashed")).WillOnce(Return(true));
    EXPECT_CALL(repo, findById(1u)).WillOnce(Return(std::nullopt));

    LoginUser useCase(repo, hasher);
    EXPECT_FALSE(useCase.execute("alice@example.com", "secret").has_value());
}
