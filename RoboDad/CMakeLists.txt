project("RoboDad")

include(FetchContent)

# ── Standalone ASIO ───────────────────────────────────────────────────────────
# FetchContent_Populate (not MakeAvailable) so we only download the headers
# without processing ASIO's own CMakeLists.txt.
FetchContent_Declare(asio
    GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
    GIT_TAG        asio-1-30-2
    GIT_SHALLOW    TRUE
    GIT_SUBMODULES ""
)
FetchContent_GetProperties(asio)
if(NOT asio_POPULATED)
    FetchContent_Populate(asio)
endif()
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
# Crow::Crow INTERFACE target is now available

# ── INTERFACE target: Crow + standalone ASIO ──────────────────────────────────
# Do NOT define CROW_USE_BOOST — crow headers check #ifdef (not #if), so
# defining it to any value (including 0) triggers the Boost ASIO branch.
add_library(crow_asio INTERFACE)
target_include_directories(crow_asio INTERFACE
    ${asio_SOURCE_DIR}/asio/include
)
target_compile_definitions(crow_asio INTERFACE ASIO_STANDALONE)
target_link_libraries(crow_asio INTERFACE Crow::Crow)

# ── Application-level headers (your own code under includes/server/) ──────────
add_library(server_headers INTERFACE)
target_include_directories(server_headers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/server
)

# ── External dependencies ─────────────────────────────────────────────────────
find_package(Threads REQUIRED)
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(PostgreSQL REQUIRED)
    find_package(CURL REQUIRED)
endif()


# ── DDD layers (inner → outer) ────────────────────────────────────────────────
add_subdirectory(src)

# ── Composition root ──────────────────────────────────────────────────────────
# The executable wires all layers together; concrete infrastructure
# implementations are injected here rather than from Presentation.
add_executable(RoboDad src/main.cpp)

target_link_libraries(RoboDad PRIVATE
    roboDadPresentation
    roboDadInfrastructure
    roboDadApplication
    roboDadDomain
    Threads::Threads
)
