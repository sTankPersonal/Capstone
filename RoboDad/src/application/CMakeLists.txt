# Application layer – use cases, commands, queries, DTOs.
# Depends ONLY on the domain layer; must never reference infrastructure
# or presentation so that the dependency rule flows strictly inward.
file(GLOB_RECURSE APPLICATION_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(roboDadApplication STATIC ${APPLICATION_SOURCES})

target_link_libraries(roboDadApplication PUBLIC
    roboDadDomain    # domain entities, repository interfaces, domain services
    server_headers   # DTOs and application-level interface headers
)