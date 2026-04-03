# Create an interface library for header-only domain layer
add_library(roboDadDomain INTERFACE)

# Optionally, specify include directories
target_include_directories(roboDadDomain INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Gather all header files in the budget, finance, and user directories
file(GLOB DOMAIN_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/budget/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/finance/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/llm/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/services/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/user/*.hpp"
)

# Add the gathered headers to the interface library
target_sources(roboDadDomain INTERFACE ${DOMAIN_HEADERS})

# Domain layer has no dependencies on other layers
