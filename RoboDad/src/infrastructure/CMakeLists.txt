# Infrastructure layer – concrete implementations of domain interfaces.
# Owns: PostgreSQL repositories (via libpqxx), LLM HTTP client, HTTP server setup,
#       security utilities (JWT, password hashing).
file(GLOB_RECURSE INFRA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(roboDadInfrastructure STATIC ${INFRA_SOURCES})

target_link_libraries(roboDadInfrastructure PUBLIC
    roboDadApplication
    roboDadDomain
    server_headers
    crow_asio
    pqxx
    jwt-cpp::jwt-cpp
    OpenSSL::SSL
    OpenSSL::Crypto
    Threads::Threads
)

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(roboDadInfrastructure PUBLIC CURL::libcurl)
endif()
