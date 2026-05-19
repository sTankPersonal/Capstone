include(CTest)

# ── Google Test + Google Mock ─────────────────────────────────────────────────
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.15.2
    GIT_SHALLOW    TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# ── Domain unit tests ─────────────────────────────────────────────────────────
add_executable(domain_tests
    domain/BudgetGoalTests.cpp
    domain/FinanceTests.cpp
    domain/UserTests.cpp
)
target_link_libraries(domain_tests PRIVATE GTest::gtest_main roboDadDomain)
gtest_discover_tests(domain_tests PROPERTIES LABELS "unit")

# ── Application unit tests (repositories mocked with GMock) ──────────────────
add_executable(application_tests
    application/BudgetUseCaseTests.cpp
    application/ExpenseUseCaseTests.cpp
    application/AuthUseCaseTests.cpp
)
target_link_libraries(application_tests PRIVATE
    GTest::gmock_main
    roboDadApplication
)
gtest_discover_tests(application_tests PROPERTIES LABELS "unit")

# ── Infrastructure tests (Linux only — requires OpenSSL and libpq) ────────────
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_executable(infrastructure_unit_tests
        infrastructure/PasswordHasherTests.cpp
        infrastructure/JwtServiceTests.cpp
        infrastructure/HttpRequestTests.cpp
        infrastructure/HttpResponseTests.cpp
        infrastructure/PromptBuilderTests.cpp
    )
    target_link_libraries(infrastructure_unit_tests PRIVATE
        GTest::gtest_main
        roboDadInfrastructure
    )
    gtest_discover_tests(infrastructure_unit_tests PROPERTIES LABELS "unit")

    # Integration tests require a live PostgreSQL instance via DATABASE_URL.
    option(RUN_INTEGRATION_TESTS "Build and register DB integration tests" OFF)
    if(RUN_INTEGRATION_TESTS)
        add_executable(infrastructure_integration_tests
            infrastructure/integration/PostgresBudgetRepositoryTests.cpp
            infrastructure/integration/PostgresUserRepositoryTests.cpp
        )
        target_link_libraries(infrastructure_integration_tests PRIVATE
            GTest::gtest_main
            roboDadInfrastructure
        )
        gtest_discover_tests(infrastructure_integration_tests PROPERTIES LABELS "integration")
    endif()
endif()
