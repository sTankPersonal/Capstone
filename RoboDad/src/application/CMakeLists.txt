file(GLOB_RECURSE APPLICATION_SOURCES *.cpp)

add_library(roboDadApplication OBJECT ${APPLICATION_SOURCES})

# Application depends on domain layer
target_link_libraries(roboDadApplication PRIVATE roboDadDomain)
