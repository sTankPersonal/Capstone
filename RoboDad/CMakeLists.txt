project("RoboDad")

include(FetchContent)

# ── Standalone ASIO ───────────────────────────────────────────────────────────
# SOURCE_SUBDIR points to the header-only path so MakeAvailable skips ASIO's root CMakeLists.txt.
FetchContent_Declare(asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
    GIT_SHALLOW    TRUE
    GIT_SUBMODULES ""
    SOURCE_SUBDIR  "asio/include"
)
FetchContent_MakeAvailable(asio)
# asio_SOURCE_DIR is now set; headers live at asio_SOURCE_DIR/asio/include/

# Crow's cmake/Findasio.cmake calls find_package(asio) and requires ASIO_INCLUDE_DIR
# to be set. Point it at our fetched copy before MakeAvailable(crow) runs.
set(ASIO_INCLUDE_DIR ${asio_SOURCE_DIR}/asio/include CACHE PATH "" FORCE)

# ── Crow HTTP framework ────────────────────────────────────────────────────────
# FetchContent_MakeAvailable processes Crow's CMakeLists.txt, which provides
# the Crow::Crow INTERFACE target pointing at its include/ directory.
set(CROW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(CROW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(CROW_ENABLE_SSL     OFF CACHE BOOL "" FORCE)
FetchContent_Declare(crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
    GIT_TAG        v1.2.0
    GIT_SHALLOW    TRUE
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(crow)

# ── libpqxx ───────────────────────────────────────────────────────────────────
# Build from source so the ABI matches gcc-14; apt's libpqxx-dev was compiled
# with an older toolchain and lacks the std::source_location exception ctors.
set(SKIP_BUILD_TEST ON  CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(pqxx
    GIT_REPOSITORY https://github.com/jtv/libpqxx.git
    GIT_TAG        7.9.2
    GIT_SHALLOW    TRUE
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(pqxx)
# Crow::Crow INTERFACE target is now available

# ── INTERFACE target: Crow + standalone ASIO ──────────────────────────────────
# Do NOT define CROW_USE_BOOST — crow headers check #ifdef (not #if), so
# defining it to any value (including 0) triggers the Boost ASIO branch.
add_library(crow_asio INTERFACE)
target_include_directories(crow_asio INTERFACE
    ${asio_SOURCE_DIR}/asio/include
)
target_compile_definitions(crow_asio INTERFACE ASIO_STANDALONE)
target_compile_options(crow_asio INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
target_link_libraries(crow_asio INTERFACE Crow::Crow)

# ── Application-level headers (your own code under includes/server/) ──────────
add_library(server_headers INTERFACE)
target_link_libraries(server_headers INTERFACE crow_asio)
target_include_directories(server_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/sharedKernel/domain
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/sharedKernel/infrastructure
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/sharedKernel/infrastructure/security
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/sharedKernel/utilities
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/sharedKernel/application
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/ports
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/aggregateRoots
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/entities
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/valueObjects
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/valueObjects/ids
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/referenceEntities
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/domain/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/infrastructure/persistence
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/users/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/users/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/users/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/users/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/chatSessions/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/chatSessions/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/chatSessions/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/chatSessions/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/personas/commands
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/personas/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/personas/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/personas/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/currencies/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/currencies/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/currencies/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/transactionCategories/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/transactionCategories/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/transactionCategories/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/countries/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/countries/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/countries/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/languages/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/languages/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/languages/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/employmentStatuses/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/employmentStatuses/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/employmentStatuses/dtos
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/messageSenders/queries
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/messageSenders/services
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server/application/references/messageSenders/dtos
)

# ── jwt-cpp ───────────────────────────────────────────────────────────────────
set(JWT_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(JWT_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
FetchContent_Declare(jwt-cpp
    GIT_REPOSITORY https://github.com/Thalhammer/jwt-cpp.git
    GIT_TAG        v0.7.0
    GIT_SHALLOW    TRUE
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(jwt-cpp)

# ── External dependencies ─────────────────────────────────────────────────────
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(CURL REQUIRED)
endif()


# ── DDD layers (inner → outer) ────────────────────────────────────────────────
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

# ── Composition root ──────────────────────────────────────────────────────────
# The executable wires all layers together; concrete infrastructure
# implementations are injected here rather than from Presentation.
add_executable(RoboDad src/main.cpp src/Server.cpp)

target_link_libraries(RoboDad PRIVATE
    roboDadPresentation
    roboDadInfrastructure
    roboDadApplication
    roboDadDomain
    Threads::Threads
)
