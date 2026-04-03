project ("RoboDad")

# Include top-level headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add source directory to handle object libraries
add_subdirectory("src")

# Define the executable
add_executable(RoboDad
	src/main.cpp
)

# Include Crow + ASIO headers AFTER target is created
target_include_directories(RoboDad PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
    ${CMAKE_CURRENT_SOURCE_DIR}/includes/asio
)

# Link the combined library 
#target_link_libraries(RoboDad PRIVATE RoboDadLib)

# Threads for async/networking
find_package(Threads REQUIRED)
target_link_libraries(RoboDad PRIVATE Threads::Threads)