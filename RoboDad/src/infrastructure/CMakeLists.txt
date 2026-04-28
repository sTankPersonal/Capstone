# Infrastructure layer – concrete implementations of domain interfaces.
# Owns: PostgreSQL repositories, LLM HTTP client, HTTP server setup,
#       security utilities (JWT, password hashing).
# Depends on domain + application (implements their interfaces) plus
# all third-party libraries so those details stay out of inner layers.
file(GLOB_RECURSE INFRA_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(roboDadInfrastructure STATIC ${INFRA_SOURCES})

target_link_libraries(roboDadInfrastructure PUBLIC
    # Inner DDD layers (implements interfaces defined here)
    roboDadApplication
    roboDadDomain

    # Header trees
    server_headers
    crow_asio          # HttpServer.cpp uses Crow

    # Database service
    PostgreSQL::PostgreSQL

    # LLM chat service – LlamaClient speaks to the local Ollama/llama.cpp
    # REST endpoint over HTTP; libcurl is the transport.
    CURL::libcurl

    # Threading required by ASIO and Crow
    Threads::Threads
)