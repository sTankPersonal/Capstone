#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include "infrastructureServices/UserRepository.h"
#include "application/auth/LoginUser.h"
#include "application/auth/RegisterUser.h"
#include "user/EmploymentStatus.hpp"
#include "user/PersonalInfo.hpp"
#include "user/User.hpp"

using ::testing::Return;
using ::testing::_;

class MockUserRepository : public UserRepository {
public:
    MOCK_METHOD(User,                  create,    (const User&), (override));
    MOCK_METHOD(std::optional<User>,   findById,  (uint32_t),    (override));
    MOCK_METHOD(std::vector<User>,     findAll,   (),            (override));
    MOCK_METHOD(bool,                  update,    (const User&), (override));
    MOCK_METHOD(bool,                  remove,    (uint32_t),    (override));
};

TEST(RegisterUserTest, ExecuteCreatesUserFromPersonalInfo) {
    MockUserRepository repo;
    PersonalInfo info("Alice", 30, EmploymentStatus::Employed);
    User expected(1u, info, {});
    EXPECT_CALL(repo, create(_)).WillOnce(Return(expected));

    RegisterUser useCase(repo);
    User result = useCase.execute(info);

    EXPECT_EQ(result.getId(), 1u);
    EXPECT_EQ(result.getPersonalInfo().getName(), "Alice");
}

TEST(LoginUserTest, ExecuteReturnsUserWhenFound) {
    MockUserRepository repo;
    PersonalInfo info("Alice", 30, EmploymentStatus::Employed);
    User expected(1u, info, {});
    EXPECT_CALL(repo, findById(1u)).WillOnce(Return(std::optional<User>{expected}));

    LoginUser useCase(repo);
    auto result = useCase.execute(1u);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->getId(), 1u);
    EXPECT_EQ(result->getPersonalInfo().getName(), "Alice");
}

TEST(LoginUserTest, ExecuteReturnsNulloptWhenUserNotFound) {
    MockUserRepository repo;
    EXPECT_CALL(repo, findById(99u)).WillOnce(Return(std::optional<User>{}));

    LoginUser useCase(repo);
    EXPECT_FALSE(useCase.execute(99u).has_value());
}
