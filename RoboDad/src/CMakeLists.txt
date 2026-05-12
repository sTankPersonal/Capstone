# DDD layers processed inside-out so each layer's CMake target
# exists before the outer layer tries to link against it.
add_subdirectory(domain)
add_subdirectory(application)
add_subdirectory(infrastructure)
add_subdirectory(presentation)
