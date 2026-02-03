file(GLOB_RECURSE INFRA_SOURCES *.cpp)

add_library(roboDadInfrastructure OBJECT ${INFRA_SOURCES})

# Infrastructure depends on domain + application layers
target_link_libraries(roboDadInfrastructure PRIVATE
    roboDadDomain
    roboDadApplication
)
