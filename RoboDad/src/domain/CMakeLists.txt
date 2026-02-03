# Gather all cpp files in domain
file(GLOB_RECURSE DOMAIN_SOURCES *.cpp)

# Create an object library for aggregation
add_library(roboDadDomain OBJECT ${DOMAIN_SOURCES})

# Domain layer has no dependencies on other layers
