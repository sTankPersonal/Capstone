#Add subdirectories for each DDD layer
add_subdirectory("application")
add_subdirectory("domain")
add_subdirectory("infrastructure")
add_subdirectory("presentation")

# Combine all layers into a single library
add_library(RoboDadLib STATIC
	$<TARGET_OBJECTS:roboDadDomain>
    $<TARGET_OBJECTS:roboDadApplication>
    $<TARGET_OBJECTS:roboDadInfrastructure>
    $<TARGET_OBJECTS:roboDadPresentation>
)

# Link Threads globally
find_package(Threads REQUIRED)
target_link_libraries(RoboDadLib PRIVATE Threads::Threads)