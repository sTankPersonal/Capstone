# Infrastructure layer – concrete implementations of domain interfaces.
# Owns: PostgreSQL repositories (via libpqxx), LLM HTTP client, HTTP server setup,
#       security utilities (JWT, password hashing).
file(GLOB_RECURSE INFRA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(roboDadInfrastructure STATIC ${INFRA_SOURCES})

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(roboDadInfrastructure PUBLIC
        roboDadApplication
        roboDadDomain
        server_headers
        crow_asio
        pqxx
        CURL::libcurl
        Threads::Threads
        OpenSSL::Crypto
    )
else()
    target_link_libraries(roboDadInfrastructure PUBLIC
        roboDadApplication
        roboDadDomain
        server_headers
        crow_asio
        Threads::Threads
    )
endif()
