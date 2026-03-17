# Domain layer – pure business logic.
# INTERFACE library: header-only, zero dependencies on outer layers,
# zero dependencies on any third-party library.
add_library(roboDadDomain INTERFACE)

target_include_directories(roboDadDomain INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Pull in the generated/server header tree (repository interfaces,
# value objects, domain events defined under includes/server/domain/).
target_link_libraries(roboDadDomain INTERFACE server_headers)

file(GLOB_RECURSE DOMAIN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)
target_sources(roboDadDomain INTERFACE ${DOMAIN_HEADERS})