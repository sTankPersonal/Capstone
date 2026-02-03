# Domain layer – pure business logic, no third-party dependencies.
file(GLOB_RECURSE DOMAIN_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Shared utilities compiled alongside the domain layer.
file(GLOB_RECURSE UTILITY_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/../sharedKernel/utilities/*.cpp"
)

add_library(roboDadDomain STATIC ${DOMAIN_SOURCES} ${UTILITY_SOURCES})
target_link_libraries(roboDadDomain PUBLIC server_headers)
